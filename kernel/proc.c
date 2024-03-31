#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"

// 最大的程序数
#define NPROC 1
#define NCPU 1


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


void procinit() {
    printf("procinit\n");

    // 初始化一个proc的栈,先给一个高位的虚拟地址,后续再改
    char *pa = kalloc();
    uint64 va = 0x3fffffe000;
    kvmmap(va, (uint64)pa, PGSIZE, PTE_R|PTE_W);

    PROC[0].KSTACK = va;
    memset(pa, 0, PGSIZE);



    


    // 拿到一个proc
    struct proc *p;
    struct cpu *cpu = mycpu();
    p = &PROC[0];


    // 开始配置一个proc
    // 分配一个proc id
    p->pid = 11;
    // 配置page table
    pagetable_t pagetable = (pagetable_t) kalloc();
    memset(pagetable, 0, PGSIZE);

    memset(&p->context, 0, sizeof(p->context));

    // set sp
    p->context.sp = p->KSTACK - PGSIZE;


    // 将一个二进制程序放入对应的pagetable中
    uvminit(pagetable, initcode, sizeof(initcode));
    p->sz = PGSIZE;
    
    

    // 将这个proc分配给cpu
    cpu->p = p;

    swtch(&cpu->context, &p->context);

    // proc执行完后需要重置cpu的proc
    cpu->p = 0;
    
}


void userinit() {
    printf("userinit\n");
}


void scheduler() {
    printf("scheduler\n");
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