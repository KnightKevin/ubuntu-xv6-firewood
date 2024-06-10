#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "syscall.h"


// Fetch the null-terminated string at addr from the current process.
// Return length of string, not including null, or -1 for error.
int fetchstr(uint64 addr, char *buf, int max) 
{
    struct proc *p = myproc();
    int err = copyinstr(p->pagetable, buf, addr, max);

    if (err < 0) {
        return err;
    }

    return strlen(buf);
}


static uint64 argraw(int n) {
    struct proc *p = myproc();

    switch (n)
    {
    case 0:
        return p->trapframe->a0;
    // case 1:
    //     return p->trapframe->a1;
    // case 2:
    //     return p->trapframe->a2;
    // case 3:
    //     return p->trapframe->a3;
    // case 4:
    //     return p->trapframe->a4;
    // case 5:    
    //     return p->trapframe->a5;
    }

    panic("argraw error!");

    return -1;
}

int argaddr(int n, uint64 *ip) {
    *ip = argraw(n);
    return 0;
}

int argstr(int n, char *buff, int max) {
    // todo
    uint64 addr;
    if (argaddr(n, &addr) < 0) {
        return -1;
    }
    return -1;
}


extern uint64 sys_exec(void);

static uint64 (*syscalls[])(void) = {
[SYS_exec]  sys_exec
};

void syscall() {
    int num;
    struct proc *p = myproc();

    num = p->trapframe->a7;

    if (num > 0 && num < NELEM(syscalls) && syscalls[num]) {
        p->trapframe->a0 = syscalls[num]();
    } else {
        printf("%d %s: unknown sys call %d\n", p->pid, p->name, num);
        p->trapframe->a0 = -1;
    }
}