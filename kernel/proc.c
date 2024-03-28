#include "types.h"
#include "risc.h"
#include "defs.h"
#include "proc.h"

// 最大的程序数
#define NPROC 1;

struct proc PROC[NPROC];


void procinit() {
    printf("procinit\n");
    
}


void userinit() {
    printf("userinit\n");
}


void scheduler() {
    printf("scheduler\n");
}