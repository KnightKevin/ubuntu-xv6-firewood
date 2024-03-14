.globl _entry
.text
.equ UART_BASE,0x10010000
_entry:
    # 配置堆栈，先随便弄一个，作用就是支持c语言
    li sp, 0x8000ffff

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