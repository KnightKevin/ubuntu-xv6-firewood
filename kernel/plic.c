#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "memlayout.h"

void plicinit(void) {
     *(uint32*)(PLIC + 4 * VIRTIO_IRQ) = 1;
  printf("fasdfasdf %d\n", *(uint32*)(PLIC + VIRTIO_IRQ*4));

}

void plicinithart(void) {
    int hart = cpuid();
    *(uint32 *)PLIC_SENABLE(hart) = 1 << VIRTIO_IRQ;
    printf("fffff: %d\n", PLIC_SENABLE(hart)) ;

    *(uint32 *)PLIC_SPRIORITY(hart) = 0;
    printf("fffff: %d\n", PLIC_SPRIORITY(hart)) ;

}