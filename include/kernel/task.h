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

#include <kernel/kernel.h>
#include <kernel/list.h>

/*************************************************************************************
 * Structure Name: tcb
 * Description: task controller block.
 *
 * Param:
 *   @task_sp: task stack pointer.
 *   @node: list node of tcb.
 *   @next: pointer to next tcb of the same priority in rdy list.
 *   @past_state: past state of system.
 *   @curr_state: current state of system.
 *   @prio: priority of the task.
 *   @tid: task id.
 *   @signal: signal of the task received.
 *   @dly_ticks_hi: hight 32bit ticks of delay.
 *   @dly_ticks_lo: lower 32bit ticks of delay.
 *
 ************************************************************************************/
struct tcb
{
	void *task_sp;
	struct list_node node;
	struct tcb *next;
	u8_t past_state;
	u8_t curr_state;
	u16_t prio;
	u16_t tid;
	u16_t signal;

#ifdef POS_CFG_TASK_DLY
	u32_t dly_ticks_hi;
	u32_t dly_ticks_lo;
#endif
};



#endif /* __KERNEL_TASK_H__ */