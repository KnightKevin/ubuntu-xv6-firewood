// 参考xv6的目录分布

// console.c
void consoleinit(void);
void consputc(char c);

// uart.c
void uartinit(void);
void uartputc_sync(char c);

// printf.c
void printf(char *fmt, ...);