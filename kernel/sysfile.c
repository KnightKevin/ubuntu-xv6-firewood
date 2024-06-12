#include "types.h"
#include "risc.h"
#include "proc.h"
#include "defs.h"
#include "syscall.h"
#include "param.h"



// 第一次让其执行user/init.c中的代码
// 获取用户空间的二进制代码，写入到对应proc中的内存中
uint64 sys_exec(void) {
    printf("syscall sys_exec\n");

    int i;
    // 获取path，argv（用户从控制台输入的命令字符串和若干个参数）
    char path[MAXPATH], *argv[MAXARG];


    // int i;
    uint64 uargv;

    if (argstr(0, path, MAXPATH) < 0 || argaddr(1, &uargv) < 0) {
        return -1;
    }

    memset(argv, 0, sizeof(argv));

    // todo

    for (i=0;;i++) {
        if (i >= NELEM(argv)) {
            goto bad;
        }
        
        if (fetchaddr()) {

        }
    }

    bad:
        for(i = 0;i < NELEM(argv) && argv[i] != 0; i++) {
            kfree(argv[i]);
        }
    return -1;
}

