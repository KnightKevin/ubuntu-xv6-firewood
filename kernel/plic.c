#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "memlayout.h"

void plicinit(void) {
     *(uint32*)(PLIC + 4 * VIRTIO_IRQ) = 1;
}

void plicinithart(void) {
    int hart = cpuid();
    *(uint32 *)PLIC_SENABLE(hart) = 1 << VIRTIO_IRQ;
    *(uint32 *)PLIC_SPRIORITY(hart) = 0;
}

int plic_claim() {
    int hart = cpuid();
    int irq = *(uint32*)PLIC_SCLAIM(hart);
    return irq;
}

void plic_complete(int irq) {
    int hart = cpuid();
    *(uint32*)PLIC_SCLAIM(hart) = irq;
}