#ifndef FS_H
#define FS_H


#define ROOTINO 1 // root i-number
#define DIRSIZ 14

#define BSIZE 1024 // block size

#define NDIRECT 12

#define BPB (1024*8) // Bitmap bits per block

//
#define BBLOCK(b, sb) ((b)/BPB + sb.bmapstart)

struct superblock {
    uint size; // size of file system image (blocks)
    uint ninodes;
    uint bmapstart;
    uint inodestart;
};

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14


struct dinode {
    short type;
    short major;
    short minor;
    short nlink;
    uint size;
    uint addres[NDIRECT + 1];
};

// Inodes per block
#define IPB (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb) ((i)/IPB + sb.inodestart)


struct dirent {
    short inum;
    char name[DIRSIZ];
};

#endif

