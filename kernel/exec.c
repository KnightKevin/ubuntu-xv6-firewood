#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "syscall.h"
#include "param.h"
#include "file.h"
#include "elf.h"

int exec(char *path, char **argv) {
    printf("kernel exec()\n");

    struct inode *ip;
    struct elfhdr elf;

    begin_op();

    if ((ip = namei(path)) == 0) {
        end_op();
        return -1;
    }

    // Check ELF header
    if (readi(ip, 0, &elf, 0, sizeof(elf)) != sizeof(elf)) {
        goto bad;
    }

    bad:
        panic("exec error!");
    return -1;
}