#include "types.h"
#include "risc.h"
#include "defs.h"

void* memset(void *dst, int c, uint n) {
    char *p;

    p = (char *)dst;

    uint i;
    for (i = 0; i<n; i++) {
        p[i] = c;
    }

    return dst;
}