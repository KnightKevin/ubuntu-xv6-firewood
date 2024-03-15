#include "types.h"
#include "defs.h"

void* memset(void *dst, int c, uint n) {
    char *p;

    p = (char *)dst;

    int i;
    for (i = 0; i<n; i++) {
        p[i] = c;
    }

    return dst;
}