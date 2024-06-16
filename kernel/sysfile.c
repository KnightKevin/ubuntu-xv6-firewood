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
    uint64 uargv, uarg;

    if (argstr(0, path, MAXPATH) < 0 || argaddr(1, &uargv) < 0) {
        return -1;
    }

    memset(argv, 0, sizeof(argv));

    // todo

    for (i=0;;i++) {
        if (i >= NELEM(argv)) {
            goto bad;
        }
        
        // 源码中是这样调用的fetchaddr(uargv+sizeof(uint64)*i, (uint64*)&uarg)。其中第二个参数
        // (uint64*)&uarg是以这种方式来传的，其实&uarg本身就是(uiint64*)类型的指针了。
        // 查了相关材料发现是为了代码可读性，让你一眼就看清楚这个参数需要什么类型的指针，
        // 还有一个原因就是为了让某些编译器能通过编译，恰巧我这个编译器能编译通过。
        if (fetchaddr(uargv + sizeof(uint64)*i, &uarg) < 0) {
            goto bad;
        }

        if (uarg == 0) {
            argv[i] = 0;
            break;
        }

        argv[i] = kalloc();
        if (argv[i] == 0) {
            goto bad;
        }

        if (fetchstr(uarg, argv[i], PGSIZE) < 0) {
            goto bad;
        }

    }
    int ret = exec(path, argv);

    for(i = 0;i < NELEM(argv) && argv[i] != 0; i++) {
        kfree(argv[i]);
    }

    return ret;

    bad:
        for(i = 0;i < NELEM(argv) && argv[i] != 0; i++) {
            kfree(argv[i]);
        }
    return -1;
}

