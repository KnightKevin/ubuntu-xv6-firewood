#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "memlayout.h"

#define PX_MASK(level) (12+9*level)
#define PX(a,level) ((a&(0x1ff<<PX_MASK(level)))>>PX_MASK(level))
#define PA2PTE(pa) ((((uint64)pa)>>12)<<10)
#define PTE2PA(pte) ((((uint64)pte)>>10)<<12)

pagetable_t kernel_pagetable;

extern char etext[];

extern char trampoline[];

void kvminit() {
    // 创建内核页表
    kernel_pagetable = (pagetable_t) kalloc();
    memset(kernel_pagetable, 0, PGSIZE);

    // 将必要的设备的物理地址映射成对应的虚拟地址
    kvmmap(UART0, UART0, PGSIZE, PTE_R|PTE_W);

    kvmmap(VIRTIO0, VIRTIO0, PGSIZE, PTE_R|PTE_W);

    // map kernel text executable and read-only.
    kvmmap(KERNBASE, KERNBASE, (uint64)etext-KERNBASE, PTE_R | PTE_X);

    // map kernel data and the physical RAM we'll make use of.
    kvmmap((uint64)etext, (uint64)etext, PHYSTOP-(uint64)etext, PTE_R | PTE_W);

    kvmmap((uint64)TRAMPOLINE, (uint64)trampoline, PGSIZE, PTE_X|PTE_R);
}

void kvminithart() {
    w_satp(MAKE_SATP(kernel_pagetable));
    sfence_vma();
}

// translate a kernel virtual address to 
// a physical address. only needed for
// adresses on the stack.
// assumes va is page aligned
uint64 kvmpa(uint64 va) {

    uint64 off = va % PGSIZE;

    pte_t *pte = walk(kernel_pagetable, va, 0);
    if (pte == 0) {
        panic("kvmpa!");
    }

    if ((*pte & PTE_V) == 0) {
        panic("kvmpa!");
    }

    uint64 pa = PTE2PA(*pte);
    // 这个pa是这个物理页的第一个地址，所以va对应的完整物理地址要加上offset
    return pa + off;
}

/**
 * 根据va找到pagetable中的pte（叶子pte/0级pte）地址！！！！！，
 * alloc非0表示会生成必要的page table信息,注意！！！只构建必要的paget table。
 * 
*/
pte_t* walk(pagetable_t pagetable, uint64 va, int alloc) {
    // 按层级开始找
    int level = 2;
    int index;
    for (; level > 0; level--) {
        // 获取index;
        index = PX(va, level);
        pte_t *pte = &pagetable[index];
        
        // 判断是否有pte是否有效，若无效就根据alloc判断是否新增

        if (*pte & PTE_V) {
            // 已存在
            // 获取到下一个level的页表地址
            pagetable = (pagetable_t)PTE2PA(*pte);

        } else {
            // 不存在，看是否需要创建
            if (!alloc || (pagetable = ((pagetable_t)kalloc())) == 0) {
                return 0;
            }

            memset(pagetable, 0, PGSIZE);

            *pte = PA2PTE(pagetable) | PTE_V;
        }
    }

    // 获得叶子pte，也就是真正存va映射的物理地址的地方
    index = PX(va, level);
    return &pagetable[index];

}

void kvmmap(uint64 va, uint64 pa, uint64 sz, int perm) {
    if (mappages(kernel_pagetable, va, sz, pa, perm) != 0) {
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

    pte_t *pte;

    for (; s <= last ; s+=PGSIZE, pa+=PGSIZE) {
        // 通过va用walk()获取到叶子节点pte，如果没有就生成
        if ((pte = walk(pagetable, s, 1)) == 0) {
            return -1;
        }

        // 判断这个pte是否被分配过了
        if (*pte & PTE_V) {
            panic("remap!");
        }

        // 配置 pte flag
        //*pte| perm | PTE_V;
        *pte = PA2PTE(pa) | perm | PTE_V;
    }

    return 0;
}

void uvminit(pagetable_t pagetable, uchar *src, uint sz) {

    // 先分配内存
    uchar *mem = (uchar *)kalloc();
    memset(mem, 0, PGSIZE);

    // 把物理内存注册到页表中
    mappages(pagetable, 0, sz, (uint64)mem, PTE_R|PTE_W|PTE_X|PTE_U);

    // 将数据src写到这个内存中
    memmove(mem, src, sz);
}

void uvmfree(pagetable_t pagetable) {
    // todo 将已分配的pagetable释放
}

// Remove npages of mappings starting from va. va must be
// page-aligned. The mappings must exist.
// Optionally free the physical memory.
void uvmunmap(pagetable_t pagetable, uint64 va, uint64 npages, int do_free)
{
  // todo uvmunmap
}

pagetable_t uvmcreate() {
    pagetable_t pagetable;
    pagetable = (pagetable_t) kalloc();
    if (pagetable == 0) {
        return 0;
    }

    memset(pagetable, 0, PGSIZE);
    return pagetable;
}

uint64 walkaddr(pagetable_t pagetable, uint64 va) {
    pte_t *pte;
    pte = walk(pagetable, va, 0);

    if (pte == 0) {
        return 0;
    }

    if ((*pte & PTE_V) == 0) {
        return 0;
    }

    if ((*pte & PTE_U) == 0) {
        return 0;
    }
    return PTE2PA(*pte);
}


// Copy a null-terminated string from user space to kernel.
// Copy bytes to dst from virtual address srcva in a given page table, until a '\0', or max.
// Return 0 on success, -1 on error.
int copyinstr(pagetable_t pagetable, char *dst, uint64 srcva, uint64 max)
{
    int got_null=0;
    int n;
       

    while (got_null == 0 && max > 0) {

        uint64 va = PGROUNDDOWN(srcva);

        n = PGSIZE - (srcva-va);
        if (n > max) {
            n = max;
        }
        
        uint64 pa = walkaddr(pagetable, va);

        if (pa <= 0) {
            return -1;
        }

        char *p = (char *)(pa + (srcva - va));

        while (n > 0) {
            if (*p == '\0') {
                *dst = '\0';
                got_null = 1;
                break;
            } else {
                *dst = *p;
            }

            dst++;
            p++;
            n--;
            max--;
        }

        srcva = va + PGSIZE;
    }
    
    
    if (got_null) {
        return 0;
    } else {
        return -1;
    }
}


// Copy from user space to kernel;
// Copy len bytes to dst from virtual address srcva in a given page table.
// return 0 on success, -1 on error
int copyin(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len)
{
    uint64 n, va , pa;

    while (len > 0) {
        va = PGROUNDDOWN(srcva);

        pa = walkaddr(pagetable, va);

        if (pa <= 0) {
            return -1;
        }

        n = PGSIZE - (srcva -va);

        if (n >= len) {
            n = len;
        }

        memmove(dst, (void *)(pa + (srcva - va)), n);

        len -= n;
        dst+=n;
        srcva = va + PGSIZE;
    }


    return 0;
}