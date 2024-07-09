#include "stdarg.h"
#include "types.h"
#include "risc.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"

void printint(int xx, int base, int sign) {

}

// print to the console.
void printf(char *fmt, ...) 
{


    va_list ap;
    // 循环后去每个字符




    int i;
    va_start(ap, fmt);

    for (i=0;fmt[i]!=0;i++) {
        char c = fmt[i];

        // 如果字符不是%，那么就直接打印字符并开始下一轮循环
        if (c != '%') {
            consputc(c);
            continue;
        }

        c = fmt[++i] & 0xff;
        switch(c){
            case 'd':
                printint(va_arg(ap, int), 10, 1);
                break;
            default:
                consputc(c);
                consputc(' ');
                panic("can't be parse!");


        }
    }
}

void panic(char *s) {
    // todo 
    printf(s);

    for(;;)
        ;
}