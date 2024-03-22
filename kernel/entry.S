.globl _entry
.text
.equ UART_BASE,0x10010000
_entry:
    # 配置堆栈，作用就是支持c语言
    # stack0来自start.c
    la sp, stack0
    li a0, 1024*4
	csrr a1, mhartid
    addi a1, a1, 1
    mul a0, a0, a1
    add sp, sp, a0
    // li sp, 0x80050000

    # uart base



    # IER
    # li a1,UART_BASE+1
    # li a2,0x0
    # sb a2,0(a1)

    # # LCR
    # li a1, UART_BASE+3
    # li a2,0x10000000
    # sb a2, 0(a1)

    # # LSB
    # li a1,UART_BASE
    # li a2,0x3
    # sb a2,0(a1)

    # # MSB
    # li a1,UART_BASE+1
    # li a2,0x0
    # sb a2,0(a1)

    # # LCR
    # li a1, UART_BASE+3
    # li a2,0x00000011
    # sb a2, 0(a1)

    # # FCR
    # li a1, UART_BASE+2
    # li a2,0x00000111
    # sb a2, 0(a1)


    # # IER
    # li a1,UART_BASE+1
    # li a2,0x11
    # sb a2,0(a1)

    # li a1,UART_BASE
    # li a2,97
    # sb a2,0(a1)
    

    # start.c
    call start

spin:
    j spin