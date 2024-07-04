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
- [ ] printf记得处理%s,%d
- [x] 开始收集空余的内存，并将其注册到空闲的内存链表kmem(kalloc.c中定义的结构体)中
- [x] 定义物理内存可用最大值常量
- [x] 定义m model下sp的内存地址常量，以及栈的size
- [x] 实现memset()方法，了解他在kfree()中是做什么的
- [x] 为什么在def.h中不能用uint这个类型，很奇怪（解决办法：直接在defs.h中添加types.h的引用。具体描述：为什么xv6源码中没有引用这个types.h也不影响编译呢，那是因为在`.c`文件中引用的时候注意了引用顺序，他一定是先引types.h再引defs.h。这样做我觉得不科学，应该在defs.h中直接添加types.h的引用）
- [x] 了解为啥向上取整使用 (sz+PGSIZE-1) & ~(PGSIZE-1)，感觉是二进制中关于除法的加减法实现
- [ ] 源码中在uartputc_sync()中判断了某个标志位，满足条件才写入数据，否则就一直轮询等待，目前还没弄清楚为什么
- [x] 创建kernel pagetable
- [x] 将内核要用到的设备物理地址映射到对应的虚拟地址中
- [x] 实现kvmmap()方法,注意实现pte的flag配置(pte_r|pte_w|pte_x)
- [x] mappages()方法最后需要将物理地址转为pte并配置对应的flag
- [x] walk()待实现
- [x] 编写将level=1,2的pte转为x页表的物理地址
- [x] 封装一个宏实现物理地址转pte
- [x] __attribute__ ((aligned (16))) char stack0[4096*NCPU]; 用来定义c语言要用的栈，这个写法就是要求这个变量是16字节对齐
- [x] C语言执行前提的栈配置须要仔细看看在entry.S文件中
- [x] 配置satp
- [x] 封装一个宏，专门写入csr寄存器
- [ ] 刷新页表缓存，flush the TLB
- [x] 在kernel.ld的代码段中最后配置了一个4k对齐的用法，`. = aligned(0x1000)`可以让当前地址是0x1000的倍数，这样的作用可以在页表映射的时候完整的将代码段映射完成且不会跟下一个数据段的映射产生冲突
- [ ] 可用proc的初始化
- [x] 了解c语言中的`static inline`我看他们用在了h文件中定义了一个方法用于寄存器的操作
- [ ] 执行第一个程序
- [x] 定义一个proc结构体
- [x] 仅初始化一个proc[0]
- [ ] 配置每一个proc的栈（地址加+size(一般是4096byte)）,随便映射了一个高位的虚拟地址
- [ ] 然后开始执行这个proc[0]
- [ ] 了解一下`tp`寄存器，这个存放着当前运行的cpu id(hart id)
- [ ] 读取tp寄存器用的mv伪指令,了解一下
- [ ] 给第一个程序分配一个proc id
- [x] 给propc配置page table
- [ ] 为什么结构体cpu中的context不用指针 
- [ ] 由于没有配置ra，导致swtch.S ret后默认返回到了0x0地址(ra默认为0)去运行了，这是不对的，难怪源码中要配置一个ra地址，上面的注释写着这是proc开始运行时运行环境第一次从kernel转到user space的地址（也就是s-mode => u-mode）
- [ ] scase中有一个bit位是用来区分中断和异常的
- [ ] 还没理解forkret是做什么的，先跳过
- [x] swtch.S ret后进入forkret后报错了，初步查看应该是sp配置的不对，须要仔细检查一下
- [x] 严重代码错误：start.c跳到main.c后没有成功从m-mode转到s-mode
- [x] 严重代码错误：pte构造错误，导致页表数据错误，所以cpu寻址也会错误
- [x] trap.c ((void (*)(uint64, uint64))fn)(0, 0) 没跳过去，不晓得为什么.——不记得了， 貌似是我改了kernel.ld的配置，让trampsec按4k对齐。
- [ ] kernel.ld 中*(.text .text.*) 为啥要这样写，感觉有目的
- [ ] push_off()和push_on()是干嘛的？我第一次见使用在了中
- [ ] proc.c中的allocproc()声明为啥要用static
- [x] 经过调试proc.h userinit()中没有将initcode二进制数据写入到对应的页表地址中
- [ ] 需要掌握ecall的运行逻辑
- [ ] 调用ecall会再次回到s-mode中，肯定要保护好切换前的现场（寄存器）
- [ ] 记住哪些寄存器在发生中断的时候需要处理
- [x] s-mode再切换到u-mode的要了解处理现场的细节 
- [x] 为什么调用ecall没有跳转到stvec所指定的指令地址，根据mcause来看貌似是Load access fault，scause貌似是Instruction address misaligned.可以将源码修改成我写的样子，看哪一步出错了————要配置medeleg寄存器，须要将所有中断和异常委托给s-mode（默认是m-mode）
- [x] 为什么proc结构体中的trapframe要用指针
- [ ] usertrap()待实现
- [x] 没理解defs.h中的NELEM(x)什么意思 ---- 计算一个数组中的元素个数，这样就不用主动传数组大小了
- [ ] 须要了解elf文件格式了，因为exec()须要从硬盘中获取elf格式的数据了
- [x] direct block number是inode.addrs[]的索引！！！！！！！
- [x] inode.address[12+1] 十二（怎么有的地方说11个）个直接索引，外加一个简介索引，如果文件占用超过了12个direct inode number，那么剩下的block number就放在indirect inode block number中，inode.arrress[indirect inode number]指向一个data block，它每4字节表示一个block number，一共256个
- [ ] virtio_disk.c中 `*R(VIRTIO_MMIO_QUEUE_PFN) = ((uint64)disk.pages) >> PGSHIFT` 是什么意思
- [ ] virtio种MMIO类型的transpot中queue size == queue num.
- [ ] disk.desc[idx[0]].next = idx[1]; 为什么是指向这个idx[1]
- [ ] kernelvec 为什么要用sret，暂时还不理解