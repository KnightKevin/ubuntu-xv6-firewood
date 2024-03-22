#include "types.h"
#include "risc.h"
#include "defs.h"

void main() {

    consoleinit();
    printf("\n");
    printf("kernel is booting\n");

    printf("\n");

    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging



}