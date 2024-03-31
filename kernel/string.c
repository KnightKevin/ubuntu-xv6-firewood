#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"

void* memset(void *dst, int c, uint n) {
    if (dst == 0) {
        panic("address error!");
    }

    char *p;

    p = (char *)dst;

    uint i;
    for (i = 0; i<n; i++) {
        p[i] = c;
    }

    return dst;
}


void memmove(void *dst, const void *src, uint n) {
    const uchar *s = src;
    uchar *d = dst;

    *d = *s;

    while(n>0) {
        *d = *s;
        d++;
        s++;
        n--;
    }
}