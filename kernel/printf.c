#include "stdarg.h"
#include "types.h"
#include "risc.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"

static char digits[] = "0123456789abcdef";

void printint(int xx, int base, int sign) {
    int x = xx;
    char buf[16];

    // 若xx是负数，就把他变成正数，方便后续处理
    if (sign && (sign = (xx < 0))) {
        x = -xx;
    } else {
        x = xx;
    }

    int i = 0;
    do {
        buf[i++] = digits[x%base];
    } while((x/=base) != 0);

    if (sign) {
        buf[i++] = '-';
    }

    while (--i >= 0) {
        consputc(buf[i]);
    }
}

void printptr(uint64 x) {
    consputc('0');
    consputc('x');
    int i;
    for (i=0;i < (sizeof(uint64) * 2); i++,x<<=4) {
        consputc(digits[x >> ((sizeof(uint64) * 8) - 4)]);
    }
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
            case 'x':
                printint(va_arg(ap, int), 16, 1);
                break;
            case 'p':
                printptr(va_arg(ap, uint64));
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

