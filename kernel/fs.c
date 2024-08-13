#include "types.h"
#include "risc.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "file.h"
#include "buf.h"
#include "stat.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

// there should be one superblock per disk device.
struct superblock sb;

struct {
    // todo struct spinlock lock;
    struct inode inode[NINODE];
} icache;

static struct inode* iget(uint dev, uint inum);

// 从这个设备的位图中获取一个未使用的bit。根据这个bit计算出空闲的block number
// allocate a zeroed disk block.
static uint balloc(uint dev) {

    // 遍历整个设备，找到一块没用的block
    // todo 由于目前还没有格式化过磁盘，所以暂时写死sb一些数据
    sb.size = 1000;
    sb.bmapstart = 45;

    int b, bi, m;

    struct buf *bp;
    bp = 0;
    // 目前这个磁盘只有一个位图，b永远等于0，如果以后这个磁盘更大了，一块位图肯定不够了
    // 从第一个block开始找
    for (b=0; b < sb.size; b += BPB) {
        // 读取这个block所在的位图数据,目前磁盘太小，所以这个位图的块号永远是0
        bp = bread(dev, BBLOCK(b, sb));
        // bi%8 就是要拿出这个byte的哪个一个bit来参与运算
        // 开始遍历位图数据，从0位开始，第一个为0的位停下来

        for (bi = 0; b < BPB; b++) {
            m = 1 << (b%8);
            if ((bp->data[b/8] & m) == 0) {
                bp->data[b/8] |= m; // Mark block in use.
                // todo log_write(bp);
                // todo brelse(bp);
                // todo bzero(dev, b + bi);
                return b +bi; // 后面再仔细想想为什么要这么做
            }
        }
        
        // todo brelse(bp);
        panic("balloc: out of blocks");

    }


    return 0;
}

// 根据direct block number 获取这个inode中真正的data block number
// Return the disk block address of the nth block in inode ip.
static uint bmap(struct inode *ip, uint bn)
{  
    uint addr;
    if (bn < NDIRECT) {
        if ((addr = ip->addrs[bn]) == 0) {
            // 没有就，分配一个
            ip->addrs[bn] = addr = balloc(ip->dev);
        }

        return addr;
    }

    // todo bn >12 的处理
    panic("bmap: out of range");

    return -1;
}


// Read data from inode.
// 将ip里面的数据读取到dst，可以选择从哪个偏移量off读取
// 返回读取了多少字节数据，等于-1表示读取错误
// If user_dest ==1, then dst is user virtual address,otherwise, dst is kernel address.
//off: offset
//n: 须要读取多少byte数据
int readi(struct inode *ip, int user_dst, uint64 dst, uint off, uint n)
{
    struct buf *bp;
    if (off > ip->size) {
        return 0;
    }

    if (off +n > ip->size) {
        n = ip->size - off;
    }


    // 先根据offset获取到它在inode.addr[]中的inode number（bmmap()）
    // 再拿到inode.addr[block number]中的值，这个值是数据block地址（也是也是个block number）
    // bread根据block number，去block cache中取对应块的缓存buf,若没有就创建一个buf，并从磁盘中读取数据到这个buf中

    // tot用于记录总共读取了多少byte
    // m 用于记录每次从data里面读取了多少byte
    int tot,m;
    for (tot=0; tot < n; tot += m,off += m) {
        bp = bread(ip->dev, bmap(ip, off/BSIZE));

        // if (off%BSIZE == 0) {
        //     m = BSIZE;
        // } else {
        //     m = min(BSIZE - off, n-tot);
        // }
        // 这样写更简洁
        m = min(n-tot, BSIZE - (off % BSIZE));

        either_copyout(user_dst, dst, bp->data + (off % BSIZE), m);
        // todo brelse(bp);
    }

    return tot;
    
}


// Find the inode with number inum and deivce dev
// and return the in-memory copy. Does not lock
// the inode and does not read it from disk.
static struct inode* iget(uint dev, uint inum)
{
    struct inode *ip, *empty;
    // todo acquire(&icache.lock);
    // 遍历cache

    // is the inode already cached?
    empty = 0;
    for (ip = &icache.inode[0]; ip < &icache.inode[NINODE]; ip++) {
        if (ip->ref > 0 && ip->dev == dev && ip->inum == inum) {
            ip->ref++;
            // todo release(&icache.lock);
            return ip;
        }

        if (empty == 0 && ip->ref == 0) {
            empty = ip;
        }
    }

    if (empty == 0) {
        panic("iget: no inodes");
    }

    ip = empty;
    ip->dev = dev;
    ip->inum = inum;
    ip->ref = 1;
    ip->valid = 0;
    // todo release(&icache.lock);
    
    return ip;
}

// 我也不知道怎么解释这个方法，注意name参数，他是获取第一个有效的目录名称
// 第一个目录名称后面的目录结构（注意结果是不带'/'开始的字符串）
// 若返回0，则表示没有目录/文件了
// skipelem("/a/b/c")="b/c", setting name = "a"
// skipelem("")=0, setting name = ""
static char* skipelem(char* path, char *name)
{
    char *s;

    int len;

    while(*path == '/') {
        path++;
    }

    if (*path == 0) {
        return 0;
    }

    s = path;

    while (*path != '/' && *path != 0) {
        path++;
    }

    len = path - s;

    // 修改name的值
    if (len > DIRSIZ) {
        memmove(name, s, DIRSIZ);
    } else {
        memmove(name, s, len);

        // 一定要在末尾加一个0，不然就不是字符串了
        name[len] = 0;
    }

    while(*path == '/') {
        path++;
    }

    return path;
    
}

static struct inode* namex(char *path, int nameiparent, char *name)
{
    struct inode *ip, *next;
    if (*path == '/') {
        ip = iget(ROOTDEV, ROOTINO);
    } else {
        // todo
        ip = 0;
    }

    while ((path=skipelem(path, name))!=0) {

        // lock the given inode.
        // reads inode from disk if necessary.
        ilock(ip);

        // todo 处理ip.type=1的处理

        if (nameiparent && *path == '\0') {
            // todo
            return ip;
        }

        if ((next = dirlookup(ip, name, 0)) == 0) {
            // todo iunlockput(ip);
            return 0;
        }

        // todo iunlockput(ip);
        ip = next;
    }

    return ip;
}

struct inode* namei(char *path)
{
    char name[DIRSIZ];
    return namex(path, 0, name);
}

struct inode* dirlookup(struct inode *dp, char *name, uint *poff)
{
    if (dp->type != T_DIR) {
        panic("dirlookup not DIR");
    }

    uint off;
    struct dirent de;

    for (off = 0; off < dp->size; off += sizeof(de)) {
        if (readi(dp, 0, (uint64)&de, off, sizeof(de)) != sizeof(de)) {
            panic("dirlookup read");
        }

        if (de.inum == 0) {
            continue;
        }

        // todo

    }

    return 0;
}

// reads the inode from disk if necessary.
void ilock(struct inode *ip)
{

    // todo 由于目前还没有格式化过磁盘，所以暂时写死sb一些数据
    sb.ninodes = 200;
    sb.inodestart = 32;

    // todo
    if (ip ==0 || ip->ref < 1) {
        panic("ilock");
    }

    struct buf *bp;
    struct dinode *dip;
    // todo acquiresleep(&ip->lock);

    // need to read inode from disk
    if (ip->valid == 0) {
        bp = bread(ip->dev, IBLOCK(ip->inum, sb));

        dip = (struct dinode*)bp->data + (ip->inum%IPB);
        ip->type = dip->type;
        ip->major = dip->major;
        ip->minor = dip->minor;
        ip->nlink = dip->nlink;
        ip->size = dip->size;
        memmove(ip->addrs, dip->addres, sizeof(ip->addrs));
        brelse(bp);
        ip->valid = 1;
        if (ip->type == 0) {
            panic("ilock: no type\n");
        }
    }



}

int namecmp(const char *s, const char *t)
{
    return strncmp(s, t, DIRSIZ);
}