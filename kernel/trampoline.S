
.section trampsec

.global trampoline
.align 4
.global userret

trampoline:
    li a3,1

userret:
    li a3,2