#define ROOTINO 1 // root i-number
#define DIRSIZ 14

#define BSIZE 1024 // block size

#define NDIRECT 12

#define BPB (1024*8) // Bitmap bits per block

//
#define BBLOCK(b, sb) ((b)/BPB + sb.bmapstart)

struct superblock {
    uint size; // size of file system image (blocks)
    uint bmapstart;
};