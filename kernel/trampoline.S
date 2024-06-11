
.section trampsec

.global trampoline

trampoline:
.align 4
.global uservec
uservec:
    # todo save user registers in TRAPFRAME
    

    # todo restore kernel stack pointer from p->trapframe->kernel_sp

    
    # jump to usertrap(), which does not return
    csrrw a0, sscratch, a0


    ld t1, 0(a0)
    ld sp, 8(a0)

    # 须要将u-mode中的寄存器存到proc中的trapframe
    sd a7, 168(a0)

    csrr t0, sscratch

    // save to proc->trapframe->a0
    sd t0, 112(a0)

    ld t0, 16(a0)


    csrw satp, t1
    sfence.vma zero, zero

    jr t0

.global userret
userret:
    # userret(TRAPFRAME, pagetable)
    # switch kernel to user.
    # usertrapret calls here.
    # a0: TRAPFRAME, in user page table
    # a1: user pagetable for user use


    csrw satp, a1
    sfence.vma zero, zero

    csrw sscratch, a0

    # ecall 0x0000000000000014
    # 0x0000003ffffff022
    sret