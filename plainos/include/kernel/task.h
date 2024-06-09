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

#ifndef __KERNEL_TASK_H__
#define __KERNEL_TASK_H__

#include <config.h>
#include <types.h>
#include "kernel.h"

typedef void *pl_tid_t;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************
 * 函数名称: pl_task_schedule_lock
 * 描述: 禁用任务调度器。
 *
 * 参数:
 *   无
 *
 * 返回值:
 *   无
 ***********************************************************************************/
void pl_task_schedule_lock(void);

/**********************************************************************************
 * 函数名称: pl_task_schedule_unlock
 * 描述: 启用任务调度器。
 *
 * 参数:
 *   无
 *
 * 返回值:
 *   无
 ***********************************************************************************/
void pl_task_schedule_unlock(void);

/**********************************************************************************
 * 函数名称: pl_task_create_with_stack
 * 描述: 使用指定的栈空间创建一个任务。
 *
 * 参数:
 *   @name: 任务名称（可选）。
 *   @task: 任务函数原型，形如 "int task(int argc, char *argv[])"。
 *   @prio: 任务优先级，若为0则采用其父任务的优先级（可选）。
 *   @stack: 任务的栈空间（必须提供）。
 *   @stack_size: 栈大小（必须指定）。
 *   @argc: 参数个数（可选）。
 *   @argv: 参数数组（可选）。
 *
 * 返回值:
 *   任务ID。
 ***********************************************************************************/
pl_tid_t pl_task_create_with_stack(const char *name, main_t task, u16_t prio,
                                   void *stack, size_t stack_size,
                                   int argc, char *argv[]);

/**********************************************************************************
 * 函数名称: pl_task_create
 * 描述: 创建一个任务并自动分配栈空间。
 *
 * 参数:
 *   @name: 任务名称（可选）。
 *   @task: 任务函数原型，形式为 "int task(int argc, char *argv[])"。
 *   @prio: 任务优先级，若为0则继承其父任务的优先级（可选）。
 *   @stack_size: 栈大小（必须指定）。
 *   @argc: 参数数量（可选）。
 *   @argv: 参数列表（可选）。
 *
 * 返回值:
 *   任务ID。
 ***********************************************************************************/
pl_tid_t pl_task_create(const char *name, main_t task, u16_t prio,
                        size_t stack_size, int argc, char *argv[]);

/**********************************************************************************
 * 函数名称: pl_task_delay_ticks
 * 描述: 延迟指定的时钟周期函数。
 *
 * 参数:
 *  @ticks: 需要延迟的时钟周期数。
 *
 * 返回值:
 *  无。
 ***********************************************************************************/
void pl_task_delay_ticks(u32_t ticks);

/**********************************************************************************
 * 函数名称: pl_task_join
 * 描述: 等待指定任务退出。
 *
 * 参数:
 *  @tid: 任务ID。
 *  @ret: 等待任务的返回值（传出参数）。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_task_join(pl_tid_t tid, int *ret);

/**********************************************************************************
 * 函数名称: pl_task_pend
 * 描述: 挂起任务。
 *
 * 注意:
 *   不应在临界区保护函数`pl_port_enter_critical`内部使用此函数。
 *
 * 参数:
 *  @tid: 任务ID，如果为NULL，则挂起当前任务自身。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_task_pend(pl_tid_t tid);

/**********************************************************************************
 * 函数名称: pl_task_resume
 * 描述: 恢复任务执行。
 *
 * 注意:
 *   不应在临界区保护函数`pl_port_enter_critical`内部使用此函数。
 *
 * 参数:
 *  @tid: 任务ID。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_task_resume(pl_tid_t tid);

/**********************************************************************************
 * 函数名称: pl_task_restart
 * 描述: 重启一个任务。
 *
 * 注意:
 *   不应在临界区保护函数`pl_port_enter_critical`内部使用此函数。
 *
 * 参数:
 *  @tid: 任务ID。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_task_restart(pl_tid_t tid);

/**********************************************************************************
 * 函数名称: pl_task_has_exited
 * 描述: 检查指定任务是否已退出。
 *
 * 参数:
 *  @tid: 任务标识符，类型为 pl_tid_t。
 *
 * 返回值:
 *  若任务已退出则返回true，否则返回false。
 ***********************************************************************************/
bool pl_task_is_exit(pl_tid_t tid);

/**********************************************************************************
 * 函数名称: pl_task_kill
 * 描述: 终止一个任务。
 *
 * 参数:
 *  @tid: 任务ID。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_task_kill(pl_tid_t tid);

/**********************************************************************************
 * 函数名称: pl_task_get_syscount
 * 描述: 获取系统计数器（syscount）的值，通常与系统时钟节拍（systick）相关。
 *
 * 参数:
 *  @c: 想要获取的计数器类型或具体计数。
 *
 * 返回值:
 *  成功时返回大于等于0的计数值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_task_get_syscount(u64_t *c);

/**********************************************************************************
 * 函数名称: pl_task_get_cpu_rate_count
 * 描述: 获取CPU利用率统计信息。
 *   运行在基于systick的系统上。
 *
 * 参数:
 *  @cup_rate_base: CPU利用率计算的基础计数。
 *  @rate_useful: CPU有效使用率的计数。
 *
 * 返回值:
 *  成功时返回大于等于0的值表示成功获取数据，失败时返回小于0的值。
 ***********************************************************************************/
int pl_task_get_cpu_rate_count(u32_t *rate_base, u32_t *rate_useful);

/**********************************************************************************
 * 函数名称: pl_task_get_cpu_rate
 * 描述: 计算并获取CPU利用率。
 *   应用于基于systick的系统环境。
 *
 * 参数:
 *  @int_part: CPU利用率的整数部分。
 *  @deci_part: CPU利用率的小数部分。
 *
 * 返回值:
 *  成功时返回大于等于0的值，表示CPU利用率获取成功；失败时返回小于0的值。
 ***********************************************************************************/
int pl_task_get_cpu_rate(u32_t *int_part, u32_t *deci_part);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_TASK_H__ */
