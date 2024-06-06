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

    // 获取path，argv（用户从控制台输入的命令字符串和若干个参数）
    char path[MAXPATH], *argv[MAXARG];

    int i;
    uint64 uargv, uarg;

    if (argstr(0, path, MAXPATH) < 0 || argint(1, &uargv)) {
        return -1;
    }


    return -1;
}

