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
- [x] 答应一个字符
- [ ] UART芯片是显示字符的关键，可能需要稍微了解一下
- [ ] 要不要实现一个最简单的printf方法，方便以后打印文字
- [ ] 开始收集空余的内存，并将其注册到空闲的内存链表中

