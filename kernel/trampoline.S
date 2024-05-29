
.section trampsec

.global trampoline
.global userret

trampoline:
.align 4
.global uservec
uservec:
    # todo save user registers in TRAPFRAME
    li t0,234

    # todo restore kernel stack pointer from p->trapframe->kernel_sp

    
    # jump to usertrap(), which does not return
    jr t0;

userret:
    # userret(TRAPFRAME, pagetable)
    # switch kernel to user.
    # usertrapret calls here.
    # a0: TRAPFRAME, in user page table
    # a1: user pagetable for user use
    csrw satp, a1
    sfence.vma zero, zero

    li sp, 0x1000
    # ecall 0x0000000000000014
    # 0x0000003ffffff00e
    sret