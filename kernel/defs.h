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
void uvminit(pagetable_t pagetable, uchar *src, uint sz);
void uvmfree(pagetable_t pagetable);
pagetable_t uvmcreate(void);
void uvmunmap(pagetable_t, uint64, uint64, int);

// string.c
void* memset(void*, int, uint);
void memmove(void *dst, const void *src, uint n);

// proc.c
int cpuid(void);
void procinit(void);
void userinit(void);
void scheduler(void);
struct cpu* mycpu(void);
struct proc* myproc(void);
pagetable_t proc_pagetable(struct proc *p);

// swtch.S
void swtch(struct context *old, struct context *new);

// trap.c
void usertrapret(void);

// syscall.c
void syscall();


// todo number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))