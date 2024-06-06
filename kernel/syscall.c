#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "syscall.h"

int argstr(int n, uint64 *ip) {
    // todo
    return -1;
}

int argaddr(int n, char *buf, int max) {
    // todo
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