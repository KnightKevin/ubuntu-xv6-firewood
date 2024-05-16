#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "memlayout.h"

// 最大的程序数
#define NPROC 1
#define NCPU 1

extern char trampoline[];


struct cpu cpus[NCPU];

struct proc PROC[NPROC];

// a user program that calls exec("/init")
// od -t xC initcode
uchar initcode[] = {
  0x17, 0x05, 0x00, 0x00, 0x13, 0x05, 0x45, 0x02,
  0x97, 0x05, 0x00, 0x00, 0x93, 0x85, 0x35, 0x02,
  0x93, 0x08, 0x70, 0x00, 0x73, 0x00, 0x00, 0x00,
  0x93, 0x08, 0x20, 0x00, 0x73, 0x00, 0x00, 0x00,
  0xef, 0xf0, 0x9f, 0xff, 0x2f, 0x69, 0x6e, 0x69,
  0x74, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

void forkret(void);

static struct proc* allocproc() {
    struct proc *p = &PROC[0];

    // 开始配置一个proc
    // 分配一个proc id
    p->pid = 11;
    // 配置page table
    p->pagetable = proc_pagetable(p);
    if (p->pagetable == 0) {
        // todo freeproc(p)
        return 0;
    }

    // set sp
    p->context.sp = p->kstack + PGSIZE;

    // 告诉swtch.S中ret后跳转到哪里去
    p->context.ra = (uint64) forkret;

    return p;
}


void procinit() {
    printf("procinit\n");

    struct proc *p = &PROC[0];

    // 初始化一个proc的栈,先给一个高位的虚拟地址,后续再改
    char *pa = kalloc();
    uint64 va = 0x3fffffe000;
    kvmmap(va, (uint64)pa, PGSIZE, PTE_R|PTE_W);

    p->kstack = va;
    
}


void userinit() {
    printf("userinit\n");

    struct proc *p;
    p = allocproc();

    

    // 将一个二进制程序放入对应的pagetable中
    uvminit(p->pagetable, initcode, sizeof(initcode));
    p->sz = PGSIZE;
    

}


void scheduler() {
    printf("scheduler\n");

    struct proc *p = &PROC[0];

    struct cpu *cpu = mycpu();


    // 将这个proc分配给cpu
    cpu->p = p;


    swtch(&cpu->context, &p->context);

    // proc执行完后需要重置cpu的proc
    cpu->p = 0;
}


int cpuid() {
    int id = r_tp();
    return id;
}

struct cpu* mycpu() {
    int id = cpuid();

    struct cpu *c = &cpus[id];

    return c;

}

struct proc* myproc() {
    // TODO push_off()

    return mycpu()->p;

    // todo push_on()
}

void forkret() {
    printf("forkret\n");

    // return to user space

    usertrapret();
}


pagetable_t proc_pagetable(struct proc *p) {
    pagetable_t pagetable;

    pagetable = uvmcreate();
    if (pagetable == 0) {
        return 0;
    }

    if (mappages(pagetable, TRAMPOLINE, PGSIZE, (uint64) trampoline, PTE_R|PTE_X) < 0) {
        uvmfree(pagetable);
        return 0;
    }

    // todo map the trapframe

    return pagetable;
}