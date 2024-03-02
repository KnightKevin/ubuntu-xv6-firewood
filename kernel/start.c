void start() {
    int a = 0;
    int b = a+1;

    // 通过mret跳转到到supervisor mode。其实是讨巧的做法，一般是从前一个Supervisor模式转换到Machine模式的调用返回，说人话： 就是s转到m，然后m现在做完事情了须要返回到s状态
}