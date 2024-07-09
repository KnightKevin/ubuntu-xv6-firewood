
K = kernel

OBJS = \
	$K/entry.o \
	$K/start.o \
	$K/main.o \
	$K/console.o \
	$K/uart.o \
	$K/printf.o \
	$K/kalloc.o \
	$K/vm.o \
	$K/string.o \
	$K/proc.o \
	$K/swtch.o \
	$K/trap.o \
	$K/trampoline.o \
	$K/syscall.o \
	$K/sysfile.o \
	$K/exec.o \
	$K/fs.o \
	$K/log.o \
	$K/bio.o \
	$K/virtio_disk.o \
	$K/kernelvec.o \
	$K/plic.o

QEMU = qemu-system-riscv64

AS = gas
CC = riscv64-linux-gnu-gcc
LD = riscv64-linux-gnu-ld
OBJDUMP = riscv64-linux-gnu-objdump

CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb -DSOL_UTIL -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib -mno-relax -I. -fno-stack-protector -fno-pie -no-pie

CPUS = 1
MEM = 128M

GDBPORT = 25000

QEMUGDB = -S -gdb tcp::25000

QEMUOPTS = -kernel $K/kernel  -machine virt -bios none -m $(MEM) -smp $(CPUS) -nographic 
QEMUOPTS += -drive file=fs.img,if=none,format=raw,id=x0
QEMUOPTS += -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0 



$K/kernel : $(OBJS)
	@echo "build kernel"
	$(LD) -T $K/kernel.ld -o $@ $^ 
	$(OBJDUMP) -S $K/kernel > $K/kernel.asm
	$(OBJDUMP) -t $K/kernel | sed '1,/SYMBOL TABLE:/d;s/ .* / /; /^$$/d' > $K/kernel.sym

# fs.img: mkfs/mkfs README $(UEXTRA) $(UPROGS)
# 	mkfs/mkfs fs.img README $(UEXTRA) $(UPROGS)


.gdbinit: .gdbinit.tmpl-riscv
	sed 's/:1234/:$(GDBPORT)/' < $^ > $@

clean: 
	rm -rf $K/*.o $K/*.d $K/kernel $K/*.asm $K/*.sym .gdbinit

qemu: kernel/kernel
	 $(QEMU) $(QEMUOPTS)

qemu-gdb: kernel/kernel .gdbinit
	$(QEMU) $(QEMUOPTS) $(QEMUGDB)

gdb: .gdbinit
	riscv64-unknown-elf-gdb -x $<

# 导出设备树,方便查看设备的内存映射，中断号等等
qemu-dt: kernel/kernel
	$(QEMU) $(QEMUOPTS) -machine dumpdtb=virt.dtb
