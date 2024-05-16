#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "memlayout.h"

extern char trampoline[];

extern char userret[];

void usertrapret(void) {
    // todo 关中断

    // set S Previous Privilege mode to User
    uint64 x = r_sstatus();

    x &= ~SSTATUS_SPP;
    x |= SSTATUS_SPIE; // enable interrupts in user mode
    w_sstatus(x);

    struct proc *p = myproc();


    uint64 satp = MAKE_SATP(p->pagetable);



    // jump to trampoline.S at the top of memory.
    // 由于我们获取到的userret和trampoline事物理地址，但是我们现在处于虚拟地址中
    // 所以我须要手动转换到虚拟地址
    uint64 fn = TRAMPOLINE + (userret - trampoline);

    ((void (*)(uint64, uint64))fn)(TRAPFRAME, satp);

}