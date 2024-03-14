#include "defs.h"
#include "types.h"

#define MAX_ADDRESS 0x8000000

extern char end[];

void freerange(void *pa_start, void *pa_end);

void kinit() {
    printf("kinit\n");
    // 开始收集最后空闲的内存
    // 空闲的内存开始地址是内核文件结束的位置
    freerange(end, (void *) 123);
}

// 从一段地址范围，收集可用的内存
void freerange(void *pa_start, void *pa_end) {
    printf("freerange");
}