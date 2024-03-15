# xv6-firewood

- [x] 新起一个项目xv6-firewood。
- [x] 用makefile编译entry.s
- [x] 编写kernel.ld用来链接最后生成的kernel
- [x] 用makefile编译 start.c
- [x] entry.s用来跳转到start.c中的start()方法
- [x] 编写ld配置文件，将上述目标文件形成一个完整的内核文件。使其运行到start.c的start()方法中。
- [x] c语言中执行汇编语句
- [x] 通过mret跳转到main.c中的main方法
- [x] 通过某个寄存器查看当前所处的特权模式是哪个，经过查阅文档没找到，网上的资料显示，这是risc-v有意为之
- [x] 打印一个字符
- [x] UART芯片是显示字符的关键，可能需要稍微了解一下
- [x] 编写一个uart.c文件，用于uart的开发
- [x] 实现一个最简单的printf方法，方便以后打印文字，不支持格式化输出(不支持%d,%x,%p,%s)
- [ ] printf记得要处理backspace键的处理逻辑
- [ ] 开始收集空余的内存，并将其注册到空闲的内存链表中
- [x] 定义物理内存可用最大值常量
- [ ] 定义m model下sp的内存地址常量，以及栈的size
- [ ] 实现memset()方法，了解他在kfree()中是做什么的
- [ ] 为什么在def.h中不能用uint这个类型，很奇怪
- [ ] 了解为啥向上取整使用 (sz+PGSIZE-1) & ~(PGSIZE-1)，感觉是二进制中关于除法的加减法实现
- [ ] 源码中在uartputc_sync()中判断了某个标志位，满足条件才写入数据，否则就一直轮询等待，目前还没弄清楚为什么

