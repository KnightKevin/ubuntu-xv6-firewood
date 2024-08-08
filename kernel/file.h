#include "fs.h"

#ifndef FILE_H
#define FILE_H

// in-memory copy of an inode
struct inode {
    uint dev; // Device number
    uint inum; // Inode number
    int ref; // Reference count
    uint size;

    int valid; // inode has been read from disk?

    short type;

    // 这个inode拥有那些数据块block number。
    // 每个addrs[]里面存的是具体的block number
    // 前12个是直接存取的，从第13个开始就是间接了，就比如说这我要读取inode的第15个数据，就需要先获取addrs[12]
    // 这个表示后面的block number存在addrs[12]所指向的内存地址里面，须要写代码转一下，前面12个是方便存取的
    uint addrs[NDIRECT + 1];
};

#endif