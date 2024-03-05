
// 在这申明，然后再main.c里面定义，自然是ld会帮你做的事情
void main();

void start() {
    asm volatile("csrw mstatus, %0" : : "r" (0b100000000000));
    
    // 通过mret跳转到到supervisor mode。其实是讨巧的做法，一般是从前一个Supervisor模式转换到Machine模式的调用返回，说人话： 就是s转到m，然后m现在做完事情了须要返回到s状态
    
    // 配置mepc寄存器，地址就是main.c中的main方法地址
    // csrwi mepc, main
    //asm volatile("csrwi mepc, %0" : : "r"(main));
    asm volatile("csrw mepc, %0" : : "r" (main));

    // 执行mret, 他会跳转到mepc寄存器所设置的地址去执行
    asm volatile("mret");

}