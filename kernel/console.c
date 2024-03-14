#include "defs.h"

void consoleinit(void) {
    uartinit();
}

void consputc(char c) {
    uartputc_sync(c);
}