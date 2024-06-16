#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "syscall.h"
#include "param.h"
#include "file.h"

int exec(char *path, char **argv) {
    printf("kernel exec()\n");

    struct inode *ip;

    begin_op();

    if ((ip = namei(path)) == 0) {
        end_op();
        return -1;
    }

    printf("asdfasd");


    return -1;
}