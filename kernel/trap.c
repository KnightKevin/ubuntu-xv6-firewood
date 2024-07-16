#include "types.h"
#include "risc.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"
#include "memlayout.h"

extern char trampoline[];

extern char userret[];

extern char uservec[];

// in kernelvec.S, calls kerneltrap().
void kernelvec();

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

    // send interrupts and exceptions to kerneltrap(),
    // since we're now in the kernel
    // 将trap的处理入口便会kerneltrap，因为我们现在已经再kernel里面了
    w_stvec((uint64) kernelvec);

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

void trapinithart() {
    w_stvec((uint64) kernelvec);
}


int devintr() {
    // todo
    printf("todo: devintr\n");
    uint64 scause = r_scause();
    if ((scause & 0x8000000000000000) && (scause&0xff) == 9) {
        // 来着外部设备中断

        // 先认领
        int irq = plic_claim();
        switch(irq) {
            case VIRTIO_IRQ:
                virtio_disk_intr();
                break;
            default:
                printf("unexpected interrupt irq=%d", irq);
                panic("\n");
        }

        // 告诉plic完成中断
        plic_complete(irq);
        return 1;
    }

    return 0;
}


void kerneltrap() {
    int which_dev = 0;
    uint64 sepc = r_sepc();
    uint64 sstatus = r_sstatus();
    uint64 scause = r_scause();

    if ((sstatus & SSTATUS_SPP) == 0) {
        panic("kerneltrap: not from s-mode");
    }

    if (intr_get() != 0) {
        panic("kerneltrap: interrupts enabled!");
    }

    if ((which_dev = devintr()) == 0 ) {
        printf("scasue %p\n", scause);
        printf("sepc=%p stval=%p\n", sepc, r_stval());
        panic("kerneltrap");
    }
}