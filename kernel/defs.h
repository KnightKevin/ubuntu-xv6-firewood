// 参考xv6的目录分布
#include "types.h"

// console.c
void consoleinit(void);
void consputc(char c);

// uart.c
void uartinit(void);
void uartputc_sync(char c);

// printf.c
void printf(char *fmt, ...);
void panic(char*);

// kalloc.c
void kinit(void);
void kfree(void *pa);
void* kalloc(void);

// vm.c
void kvminit(void);
void kvminithart(void);
void kvmmap(uint64 va, uint64 pa, uint64 sz, int perm);
int  mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm);

// string.c
void* memset(void*, int, uint);

// proc.c
void procinit(void);