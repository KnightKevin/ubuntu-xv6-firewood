#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "syscall.h"


uint64 sys_exec(void) {
    printf("syscall sys_exec\n");
    return -1;
}