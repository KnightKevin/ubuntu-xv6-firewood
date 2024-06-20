// in-memory copy of an inode
struct inode {
    uint dev; // Device number
    uint inum; // Inode number
    int ref; // Reference count
    uint size;

    int valid; // inode has been read from disk?

    uint addrs[NDIRECT + 1];
};