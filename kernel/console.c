#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"

void consoleinit(void) {
    uartinit();
}

void consputc(char c) {
    uartputc_sync(c);
}