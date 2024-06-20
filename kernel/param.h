#define NCPU          1  // maximum number of CPUs
#define MAXARG        32 // max exec arguments
#define MAXPATH       128 // maximum file path name
#define NINODE       50  // maximum number of active i-nodes
#define ROOTDEV      1 // device number of file system root disk
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache