#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"
#include "param.h"

struct {
    // todo  struct spinlock lock;

    struct buf buf[NBUF];
    struct buf head;

} bcache;

void binit()
{
    struct buf* b;
    bcache.head.next = &bcache.head;
    bcache.head.prev = &bcache.head;

    for (b = bcache.buf; b < bcache.buf+NBUF; b++) {
        b->next = bcache.head.next;
        b->prev = &bcache.head;
        // todo initsleeplock(&b->lock, "buffer");
        bcache.head.next->prev = b;
        bcache.head.next = b;

    }
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// in either case, return locked buffer.
static struct buf* bget(uint dev, uint blockno)
{
    struct buf *b;

    // todo acquire(&bcache.lock);
    for (b = bcache.head.next; b != &bcache.head; b = b->next) {
        if (b->dev == dev && b->blockno == blockno) {
            b->refcnt++;

            // todo release(&bcache.lock);
            // todo acquiresleep(&b->lock);
            return b;
        }
    }

    // Recycle the least recently used (LRU) unused buffer.
    for (b = bcache.head.prev; b != &bcache.head; b = b->prev) {
        if (b->refcnt == 0) {
            b->dev = dev;
            b->blockno = blockno;
            b->valid = 0;
            b->refcnt = 1;
            // todo release(&bcache.lock);
            // todo acquiresleep(&b->lock);
            return b;
        }
    }

    panic("bget: no buffers");

    return 0;

}

struct buf* bread(uint dev, uint blockno)
{
    struct buf *b;
    b = bget(dev, blockno);

    if (!b->valid) {
        // 从磁盘中读取数据到buf中
        // 参考 https://blog.csdn.net/qq_45226456/article/details/133583975
        virtio_disk_rw(b, 0);
        b->valid = 1;
    }

    return b;
}