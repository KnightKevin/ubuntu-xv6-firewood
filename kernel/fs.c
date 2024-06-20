#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "file.h"
#include "buf.h"

// there should be one superblock per disk device.
struct superblock sb;

struct {
    // todo struct spinlock lock;
    struct inode inode[NINODE];
} icache;

static struct inode* iget(uint dev, uint inum);

// allocate a zeroed disk block.
static uint balloc(uint dev) {

    // 遍历整个设备，找到一块没用的block
    // todo 由于目前还没有格式化过磁盘，所以暂时写死sb一些数据
    sb.size = 1024*1024;
    sb.bmapstart = 44;

    int b;

    struct buf *bp;
    bp = 0;
    // 遍历每个block
    for (b=0; b < sb.size; b += BPB) {
        bp = bread(dev, BBLOCK(b, sb));
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

}


// Read data from inode.
// If user_dest ==1, then dst is user virtual address,otherwise, dst is kernel address.
//off: offset
//n: elf byte size
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

    // todo for
    bp = bread(ip->dev, bmap(ip, off/BSIZE));
    return -1;
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

static struct inode* namex(char *path, int nameiparent, char *name)
{
    struct inode *ip;
    if (*path == '/') {
        ip = iget(ROOTDEV, ROOTINO);
    } else {
        // todo
        ip = 0;
    }

    return ip;
}

struct inode* namei(char *path)
{
    char name[DIRSIZ];
    return namex(path, 0, name);
}