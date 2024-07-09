// Physical memory layout 物理内存分布图

#define UART0 0x10000000L
#define VIRTIO_IRQ 1

#define VIRTIO0 0x10001000
#define UART_IRQ 10


#define KERNBASE 0X80000000L
#define PHYSTOP KERNBASE+128*1024*1024

#define TRAMPOLINE (MAXVA - PGSIZE)
#define TRAPFRAME (TRAMPOLINE - PGSIZE)


// qemu puts programmable interrupt controller here.
#define PLIC 0xc000000L
#define PLIC_SENABLE(hart) (PLIC + 0x2080 + (hart) * 0x100)
#define PLIC_SPRIORITY(hart) (PLIC + 0x201000 + (hart) * 0x2000)