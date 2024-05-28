# 寄存器



## stap

## stvec
RISC-V中的`stvec`寄存器是用于指定异常处理的向量基地址的寄存器。当发生异常时，处理器会根据异常类型跳转到`stvec`所指定的地址处执行异常处理程序。在RISC-V中，异常处理程序的地址被组织成一个向量表，`stvec`寄存器指向这个表的起始地址。

这个表中包含了不同类型异常的处理程序地址，包括中断、陷阱和其他异常。当发生异常时，处理器会根据异常类型找到对应的处理程序地址并跳转执行。

在RISC-V中，`stvec`寄存器是一个64位寄存器，有两个不同的模式：`stvec.mode`字段指定了是直接跳转到异常处理程序的基址还是通过间接寻址进行跳转，`stvec.base`字段则指定了异常处理程序的基址。

## sepc
在RISC-V架构中，sepc寄存器（Supervisor Exception Program Counter）用于保存发生异常时的程序计数器（PC）值。当发生异常时，处理器会将导致异常的指令的地址存储到sepc寄存器中，以便异常处理程序在处理异常后能够返回到正确的指令执行位置。

sepc寄存器主要用于异常处理过程中的上下文保存和恢复。当异常发生时，处理器会将当前指令的地址保存到sepc中，并跳转到异常处理程序。在异常处理程序执行完毕后，处理器可以通过sepc中的值找到异常发生时的指令地址，并从该地址处继续执行程序。

需要注意的是，sepc寄存器是针对特权级别的，即针对监管者（Supervisor）模式的异常。在不同的特权级别下，可能会有类似功能的寄存器，如机器级别的mepc寄存器（Machine Exception Program Counter）和用户级别的uepc寄存器（User Exception Program Counter），用于保存不同特权级别下的异常发生时的指令地址。

# 指令
## ecall
调用完后会产生trap，会跳到存在stvec中指向的指令
a7-调用编号
stvec-用于