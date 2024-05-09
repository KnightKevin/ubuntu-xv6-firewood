#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "memlayout.h"

extern char trampoline[];

extern char userret[];

void usertrapret(void) {
    // todo 关中断

    // set S Previous Privilege mode to User
    uint64 x = r_sstatus();

    x &= ~SSTATUS_SPP;
    x |= SSTATUS_SPIE; // enable interrupts in user mode
    w_sstatus(x);


    // jump to trampoline.S at the top of memory.
    // 0x3ffffff000
    uint64 fn = 0x3ffffff000 ;

    ((void (*)(uint64, uint64))fn)(0, 0);

}