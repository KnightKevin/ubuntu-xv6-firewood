// 参考xv6的目录分布
#include "types.h"

// bio.c
struct buf* bread(uint, uint);
void binit(void);

// console.c
void consoleinit(void);
void consputc(char c);

// exec.c
int exec(char*, char**);

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
uint64 kvmpa(uint64);
void kvmmap(uint64 va, uint64 pa, uint64 sz, int perm);
int  mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm);
void uvminit(pagetable_t pagetable, uchar *src, uint sz);
void uvmfree(pagetable_t pagetable);
pagetable_t uvmcreate(void);
void uvmunmap(pagetable_t, uint64, uint64, int);
int copyinstr(pagetable_t, char *, uint64, uint64);
int copyin(pagetable_t, char *, uint64, uint64);
uint64 walkaddr(pagetable_t, uint64);

// string.c
void* memset(void*, int, uint);
void memmove(void *dst, const void *src, uint n);
int strlen(const char*);

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
int argint(int, int*);
int argstr(int, char*, int);
int argaddr(int, uint64 *);
int fetchstr(uint64, char*, int);
int fetchaddr(uint64, uint64*);

// log.c
void begin_op(void);
void end_op(void);

// file.c

// fs.c
struct inode* namei(char*);
int readi(struct inode*, int, uint64, uint, uint);

// virtio_disk.c
void virtio_disk_init(void);
void virtio_disk_rw(struct buf *, int);


// number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))