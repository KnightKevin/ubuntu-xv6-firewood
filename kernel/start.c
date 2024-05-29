#include "param.h"
#include "types.h"
#include "risc.h"

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

// 在这申明，然后再main.c里面定义，自然是ld会帮你做的事情
void main();

// void start() {
//     asm volatile("csrw mstatus, %0" : : "r" (0b1100000000000));
    
//     // 通过mret跳转到到supervisor mode。其实是讨巧的做法，一般是从前一个Supervisor模式转换到Machine模式的调用返回，说人话： 就是s转到m，然后m现在做完事情了须要返回到s状态
    
//     // 配置mepc寄存器，地址就是main.c中的main方法地址
//     // csrwi mepc, main
//     //asm volatile("csrwi mepc, %0" : : "r"(main));
//     asm volatile("csrw mepc, %0" : : "r" (main));

//     // 执行mret, 他会跳转到mepc寄存器所设置的地址去执行
//     asm volatile("mret");

// }

void
start()
{
  // set M Previous Privilege mode to Supervisor, for mret.
  unsigned long x = r_mstatus();
  x &= ~MSTATUS_MPP_MASK;
  x |= MSTATUS_MPP_S;
  w_mstatus(x);

  // set M Exception Program Counter to main, for mret.
  // requires gcc -mcmodel=medany
  w_mepc(((uint64) main));

  // disable paging for now.
  // w_satp(0);

  // delegate all interrupts and exceptions to supervisor mode.
  w_medeleg(0xffff);
  // w_mideleg(0xffff);
  // w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

//   // ask for clock interrupts.
//   timerinit();

//   // keep each CPU's hartid in its tp register, for cpuid().
//   int id = r_mhartid();
//   w_tp(id);

  // switch to supervisor mode and jump to main().
  asm volatile("mret");
}