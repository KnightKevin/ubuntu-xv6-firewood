#include "types.h"
#include "risc.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"

void consoleinit(void) {
    uartinit();
}

void consputc(char c) {
    uartputc_sync(c);
}