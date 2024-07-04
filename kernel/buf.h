struct buf {
    int valid;
    int disk; // does disk 'own' buf?
    uint dev;
    uint blockno;
    uint refcnt;

    struct buf *prev;
    struct buf *next;

    uchar data[BSIZE];
};