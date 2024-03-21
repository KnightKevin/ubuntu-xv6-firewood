#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "risc.h"

#define PX_MASK(level) (12+9*level)
#define PX(a,level) ((a&(0x1fff<<PX_MASK(level)))>>PX_MASK(level))

pagetable_t kernel_pagetable;

void kvminit() {
    printf("kvminit\n");
    // 创建内核页表
    kernel_pagetable = (pagetable_t) kalloc();
    memset(kernel_pagetable, 0, PGSIZE);

    // 将必要的设备的物理地址映射成对应的虚拟地址
    kvmmap(UART0, UART0, PGSIZE, PTE_R|PTE_W);

    // 将页表地址写入satp
}

void kvminithart() {
    printf("kvminithart\n");
}

/**
 * 根据va找到pagetable中的pte，alloc非0表示会生成必要的page table信息,否则直接返回0
 * 
*/
pte* walk(pagetable_t pagetable, uint64 va, int alloc) {
    // 按层级开始找
    int level = 2;
    for (level; level >=0; level--) {
        // 获取index;
        int index = PX(va, level);
        pte *pte = pagetable[index];
        
        // 判断是否有pte是否有效，若无效就根据alloc判断是否新增
    }
}

void kvmmap(uint64 va, uint64 pa, uint64 sz, int perm) {
    if (mappages(kernel_pagetable, va, sz, pa, perm)) {
        printf("page fault!");
    }
}

// 0 success,-1 walk() can not allocate a needed page-table page
int  mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm) {

    if (va >= MAXVA) {
        panic("error: va >= MAXVA!");
    }

    uint64 s = PGROUNDDOWN(va);
    uint64 last = PGROUNDDOWN(va+size-1);

    for (s ; s <= last ; s+=size) {
        // 通过va用walk()获取到叶子节点pte，如果没有就生成
        pte *pte = walk(pagetable, s, 1);

        // 判断这个pte是否被分配过了
        if (*pte & PTE_V) {
            panic("remap!");
        }

        // 配置 pte flag
        *pte| perm | PTE_V;
    }


}