#include "stdarg.h"
#include "types.h"
#include "risc.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"

// print to the console.
void printf(char *fmt, ...) 
{
    // 循环后去每个字符

    int i;

    for (i=0;fmt[i]!=0;i++) {
        char c = fmt[i];

        // 如果字符不是%，那么就直接打印字符并开始下一轮循环
        if (c != '%') {
            consputc(c);
            continue;
        }
    }
}

void panic(char *s) {
    // todo 
    printf(s);

    for(;;)
        ;
}