
K = kernel

OBJS = \
	$K/entry.o

QEMU = qemu-system-riscv64

AS = gas
CC = riscv64-linux-gnu-gcc
LD = riscv64-linux-gnu-ld
OBJDUMP = riscv64-linux-gnu-objdump

CPUS = 1
MEM = 128M

QEMUGDB = -S -gdb tcp::25000

QEMUOPTS = -kernel $K/kernel  -machine virt -bios none -m $(MEM) -smp $(CPUS) -nographic


$K/kernel : $(OBJS)
	@echo "build kernel"
	$(LD) -T $K/kernel.ld -o $@ $^ 
	$(OBJDUMP) -S $K/kernel > $K/kernel.asm

clean: 
	rm -rf $K/*.o $K/*.d $K/kernel $K/*.asm

qemu: kernel/kernel
	$(QEMU) $(QEMUOPTS)

qemu-gdb:
	$(QEMU) $(QEMUOPTS) $(QEMUGDB)

gdb:
	riscv64-unknown-elf-gdb
