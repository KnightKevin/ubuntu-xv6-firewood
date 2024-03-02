.text
_entry:
    # 配置堆栈，先随便弄一个，作用就是支持c语言
    li sp, 0x8000ffff

    # start.c
    call start

spin:
    j spin