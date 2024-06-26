#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "memlayout.h"

extern char trampoline[];

extern char userret[];

extern char uservec[];

extern int devintr();

// handle an interrupt, exception, or system call from user space.
// called from trampoline.S
// s-mode
void usertrap() {
    printf("usertrap\n");

    int which_dev = 0;

    // 判断是否来自u-mode
    if ((r_sstatus() & SSTATUS_SPP) != 0) {
        panic("usertrap: not from user mode");
    }

    struct proc *p = myproc();

    if (r_scause() == 8) {
        intr_on();
        syscall();
    } else if((which_dev = devintr()) != 0){
        // ok
    } else {
        printf("usertrap(): unexpected scause %p pid=%d\n", r_scause(), p->pid);
        printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
        p->killed = 1;
    }
}

void usertrapret(void) {
    // todo 关中断
    w_sstatus(r_sstatus() & ~SSTATUS_SIE);

    w_stvec(TRAMPOLINE + (uservec - trampoline));

    // set S Previous Privilege mode to User
    uint64 x = r_sstatus();

    x &= ~SSTATUS_SPP;
    x |= SSTATUS_SPIE; // enable interrupts in user mode
    w_sstatus(x);

    struct proc *p = myproc();

    uint64 kernel_satp = r_satp();
    p->trapframe->kernel_satp = kernel_satp;
    p->trapframe->kernel_sp = p->kstack + PGSIZE;
    p->trapframe->kernel_trap = (uint64) usertrap;


    uint64 satp = MAKE_SATP(p->pagetable);



    // jump to trampoline.S at the top of memory.
    // 由于我们获取到的userret和trampoline事物理地址，但是我们现在处于虚拟地址中
    // 所以我须要手动转换到虚拟地址
    uint64 fn = TRAMPOLINE + (userret - trampoline);

    ((void (*)(uint64, uint64))fn)(TRAPFRAME, satp);

}


int devintr() {
    // todo
    printf("todo: devintr");

    return 0;
}
