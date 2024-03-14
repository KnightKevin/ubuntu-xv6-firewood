// 参考xv6的目录分布

// console.c
void consoleinit(void);
void consputc(char c);

// uart.c
void uartinit(void);
void uartputc_sync(char c);

// printf.c
void printf(char *fmt, ...);

// kalloc.c
void kinit(void);
void kfree(void *pa);

// vm.c
void kvminit(void);
void kvminithart(void);

// string.c
void* memset(void*, int, unsigned int);