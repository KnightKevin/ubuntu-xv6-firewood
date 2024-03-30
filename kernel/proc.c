#include "types.h"
#include "risc.h"
#include "defs.h"
#include "proc.h"

// 最大的程序数
#define NPROC 1
#define NCPU 1


struct cpu cpus[NCPU];

struct proc PROC[NPROC];


void procinit() {
    printf("procinit\n");

    // 初始化一个proc的栈

    // 开始配置一个proc
    // 分配一个proc id
    // 配置page table
    


    // 拿到一个proc
    struct proc *p;
    struct cpu *cpu = mycpu();
    p = &PROC[0];

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