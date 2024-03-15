#include "types.h"
#include "defs.h"
#include "risc.h"
#include "memlayout.h"

extern char end[];

struct run {
    struct run *next;
};

struct {
    struct run *freelist;
} kmem;


void freerange(void *pa_start, void *pa_end);

void kinit() {
    printf("kinit\n");
    // 开始收集最后空闲的内存
    // 空闲的内存开始地址是内核文件结束的位置
    freerange(end, (void *) PHYSTOP);
}

// 从一段地址范围，收集可用的内存
void freerange(void *pa_start, void *pa_end) {
    printf("freerange\n");

    // 向上取整获得正真的开始地址
    char *p = (char *) PGROUNDUP((uint64)pa_start);

    for (; p+PGSIZE <= (char *)pa_end; p+= PGSIZE) {
        kfree(p);
    }
    
}

void kfree(void *pa) {
    struct run *r;
    r = (struct run *)pa;

    // 将这部分的内存数据清空(安全处理)
    memset(pa, 1, PGSIZE);

    r->next = kmem.freelist;
    kmem.freelist = r;
}