#include "defs.h"
#include "types.h"

void* memset(void *dst, int c, unsigned int n) {
    char *p;

    p = (char *)dst;

    int i;
    for (i = 0; i<n; i++) {
        p[i] = c;
    }

    return dst;
}