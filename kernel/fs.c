#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "param.h"
#include "file.h"
#include "fs.h"

struct {
    // todo struct spinlock lock;
    struct inode inode[NINODE];
} icache;

static struct inode* iget(uint dev, uint inum);

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