#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "memlayout.h"

void plicinit(void) {
    *((uint32 *) (PLIC + 4 * VIRTIO_IRQ)) = 1;
}

void plicinithart(void) {
    int hart = cpuid();
    
    
}