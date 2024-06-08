/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <port.h>
#include <kernel/assert.h>
#include <kernel/kernel.h>
#include <kernel/task.h>
#include <kernel/syslog.h>
#include "task.h"
#include "idletask.h"
#include "inittask.h"

/**
 * @brief 空闲任务函数
 * 
 * 此函数设计为一个空闲任务，在没有其他任务运行时执行。
 * 其主要目的是保持系统运行，并在没有实际工作要做时占用CPU。
 * 
 * @param argc 命令行参数数量
 * @param argv 命令行参数字符串数组
 * @return int 总是返回0
 */
static int idle_task(int argc, char *argv[])
{
    // 声明使用argc和argv以抑制编译器警告
    USED(argc);
    USED(argv);

    int ret;

    // 初始化任务，确保返回0，否则断言
    ret = pl_init_task_init();
    pl_assert(ret == 0);

    // 无限循环，作为空闲任务，在没有其他任务时运行
    while(true) {
        // 取消注释以下行以记录空闲任务活动
        // pl_early_syslog("idletask===============================================\r\n");
        // 忙等待，使用易变变量防止优化
        for (volatile int i = 0; i < 10000; i++);
    }

    return 0;
}

/**
 * 初始化空闲任务。
 * 
 * 本函数用于创建并初始化系统的空闲任务。空闲任务是在没有其他任务可以运行时，
 * CPU 会调度运行的任务。它的主要作用是保持系统运行和做一些低优先级的维护工作。
 * 
 * @return 如果初始化成功，返回0；如果创建空闲任务失败，返回-1。
 */
int pl_idle_task_init(void)
{
    // 用于存储空闲任务的任务ID
    pl_tid_t idle_taskid;

    // 调用任务创建函数创建空闲任务，参数包括任务名称、任务函数、任务优先级、任务栈大小等
    idle_taskid = pl_task_sys_create("idle_task", idle_task, PL_CFG_TASK_PRIORITIES_MAX,
                                     PL_CFG_IDLE_TASK_STACK_SIZE, 0, NULL);
    // 检查空闲任务是否创建成功
    if (idle_taskid == NULL) {
        // 如果创建失败，记录日志并返回错误码
        pl_early_syslog("idle task create failed\r\n");
        return -1;
    }

    // 如果创建成功，记录日志并返回成功码
    pl_early_syslog("idle task init successfully\r\n");
    return 0;
}
