
.section trampsec

.global trampoline
.align 4
.global userret

trampoline:
    li a3,1

userret:
    # userret(TRAPFRAME, pagetable)
    # switch kernel to user.
    # usertrapret calls here.
    # a0: TRAPFRAME, in user page table
    # a1: user pagetable for user use
    csrw satp, a1
    sfence.vma zero, zero

    sret