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
#include "mempool.h"
#include "syslog.h"
#include "task.h"
#include "idletask.h"
#include <kernel/assert.h>
#include <kernel/syslog.h>
#include <common/oschlogo.h>

/**
 * @brief 程序启动入口函数
 * 
 * 该函数是PlainOS系统的初始化入口，负责执行系统的初始化流程。
 * 包括初始化输出字符接口、日志系统初始化、内存池初始化、任务核心初始化和空闲任务初始化等关键初始化工作。
 * 初始化完成后，进入死循环，等待系统的进一步调度。
 */
void pl_callee_entry(void)
{
    int ret; // 用于存储初始化函数的返回值

    // 初始化字符输出接口，如果初始化失败，则进入死循环
    ret = pl_port_putc_init();
    if (ret < 0)
        while(1);

    // 初始化日志系统，如果初始化失败，则进入死循环
    ret = pl_syslog_init();
    if (ret < 0)
        while(1);

    // 显示操作系统字符标识
    pl_dis_os_char_logo();
    // 打印启动信息
    pl_early_syslog("PlainOS is starting...\r\n");

    // 初始化默认内存池，确保初始化成功
    ret = pl_default_mempool_init();
    pl_assert(ret == 0);

    // 初始化任务核心，确保初始化成功
    ret = pl_task_core_init();
    pl_assert(ret == 0);

    // 初始化空闲任务，确保初始化成功
    ret = pl_idle_task_init();
    pl_assert(ret == 0);

    // 进入死循环，等待系统的进一步调度
    while(true);
}
