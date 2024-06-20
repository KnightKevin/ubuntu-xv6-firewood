struct buf {
    int valid;
    uint dev;
    uint blockno;
    uint refcnt;

    struct buf *prev;
    struct buf *next;

    uchar data[BSIZE];
};