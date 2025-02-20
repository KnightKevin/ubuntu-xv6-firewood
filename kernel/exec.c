#include "types.h"
#include "risc.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"
#include "syscall.h"
#include "param.h"
#include "fs.h"
#include "file.h"
#include "elf.h"

int exec(char *path, char **argv) {
    printf("kernel exec()\n");

    struct inode *ip;
    struct elfhdr elf;
    pagetable_t pagetable = 0;
    struct proc *p = myproc();

    begin_op();

    if ((ip = namei(path)) == 0) {
        end_op();
        return -1;
    }

    // Check ELF header
    if (readi(ip, 0, (uint64)&elf, 0, sizeof(elf)) != sizeof(elf)) {
        goto bad;
    }

    if (elf.magic != ELF_MAGIC) {
        goto bad;
    }

    if ((pagetable = proc_pagetable(p)) == 0) {
        goto bad;
    }

    // Load program into memory.
    printf("todo load program into memory!");

    bad:
        panic("exec error!");
    return -1;
}