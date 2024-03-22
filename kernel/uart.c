//
//  16550a UART的驱动low-level程序
//

#include "types.h"
#include "risc.h"
#include "defs.h"
#include "memlayout.h"

#define Reg(reg) ((unsigned char *)(UART0+reg))
#define WriteReg(reg, v) (*Reg(reg) = (v))
#define ReadReg(reg) (*Reg(reg))
#define IER 1
#define LCR 3
#define LSB 0
#define MSB 1
#define FCR 2
#define THX 0
#define LSR 5

void uartinit(void) {
    // IER 关闭中断
    WriteReg(1, 0);

    // LCR 进入设置波特率模式
    WriteReg(LCR, 1<<7);

    // LSB 设置波特率为38.4k
    WriteReg(LSB, 3);

    // MSB 设置波特率为38.4k
    WriteReg(MSB, 00);

    // LCR 离开设置波特率模式
    WriteReg(LCR, 3);

    // FCR 配置FIFO
    WriteReg(FCR, 6);

    // IER 打开中断
    WriteReg(IER, 3);

}


void uartputc_sync(char c) {
    // 要判断队列是否有位置，有的化就放进去
    // char p = *((unsigned char *)(UART0+LSR));
    // while((ReadReg(LSR) & 1<<5) == 0)
    // {

    // }


    WriteReg(THX, c);
}