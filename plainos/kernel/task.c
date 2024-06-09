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

#include <config.h>
#include <errno.h>
#include <port.h>
#include <kernel/task.h>
#include <kernel/list.h>
#include <kernel/kernel.h>
#include <kernel/syslog.h>
#include <kernel/mempool.h>
#include <kernel/workqueue.h>
#include <lib/string.h>
#include "task.h"
#include "panic.h"
#include "softtimer.h"

/*************************************************************************************
 * Description: Definitions of highest priority of task.
 ************************************************************************************/
#if (CONFIG_PL_CFG_SYS_RSVD_HIGHEST_PRIOTITY < 2 || \
	 CONFIG_PL_CFG_TASK_PRIORITIES_MAX >= 512)
	#error "PL_CFG_SYS_RSVD_HIGHEST_PRIOTITY must larger 2 and\
	        PL_CFG_TASK_PRIORITIES_MAX must less 512"
#endif

/*************************************************************************************
 * structure Name: task_list
 * Description: Define task list.
 ************************************************************************************/
struct task_list {
	struct tcb *head;
	u16_t num;
};

static u32_t cpu_rate_base;
static u32_t cpu_rate_idle;
/*************************************************************************************
 * Structure Name: task_core_blk
 * Description: task core block.
 *
 * Members:
 *   @ready_list: list head array of ready tasks.
 *   @pend_list: list head of pending tasks.
 *   @delay_list: list head of delay tasks.
 *   @exit_list: list head of exit tasks(killed or exited).
 *   @timer_list: list head of soft timer.
 *   @exit_free: work for freeing wxit tcb.
 *   @curr_tcb: current context tcb.
 *   @systicks: systicks.
 *   @cpu_rate_base: cpu rate base counter.
 *   @cpu_rate_useful: cpu rate useful counter.
 *   @sched_lock_ref: schedule reference counter.
 *
 ************************************************************************************/
struct task_core_blk {
	struct task_list ready_list[CONFIG_PL_CFG_TASK_PRIORITIES_MAX + 1];
	struct task_list delay_list;
	struct list_node pend_list;
	struct list_node exit_list;
	struct list_node timer_list;
	struct pl_work exit_free;
	struct tcb *curr_tcb;
	u64_t systicks;
	u32_t cpu_rate_base;
	u32_t cpu_rate_useful;
	uint_t sched_lock_ref;
};

/*************************************************************************************
 * Global Variable Name: g_hiprio_bitmap
 *
 * Description: Obtain the highest priority through the priority bit map.
 *              Supports up to 4096 priority levels.
 ************************************************************************************/
static u32_t g_hiprio_bitmap[(CONFIG_PL_CFG_TASK_PRIORITIES_MAX + 31) / 32];

/*************************************************************************************
 * Global Variable Name: g_task_core_blk
 * Description:  task core block.
 ************************************************************************************/
static struct task_core_blk g_task_core_blk;

/*************************************************************************************
 * Function Name: pl_task_get_timer_list
 * Description: Get timer list tcb.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   @struct list_node: timer list.
 ************************************************************************************/
struct list_node *pl_task_get_timer_list(void)
{
	return &g_task_core_blk.timer_list;
}

/*************************************************************************************
 * Function Name: pl_task_get_curr_tcb
 * Description: Get current tcb.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   @struct tcb: current tcb.
 ************************************************************************************/
struct tcb *pl_task_get_curr_tcb(void)
{
	return g_task_core_blk.curr_tcb;
}

/*************************************************************************************
 * Function Name: pl_task_get_state
 * Description: get task state.
 *
 * Parameters:
 *  @tid: task id.
 *
 * Return:
 *    @int :task state
 ************************************************************************************/
int pl_task_get_state(pl_tid_t tid)
{
	return ((struct tcb *)tid)->curr_state;
}

/*************************************************************************************
 * Function Name: get_last_bit
 * Description: Get leading zero of bitmap.
 *
 * Parameters:
 *   @bitmap: bit map of priority.
 *
 * Return:
 *   @last_bit: leadzero
 ************************************************************************************/
static u8_t get_last_bit(u32_t bitmap)
{
	u8_t i = 0;

	for (i = 0; i < 32; i++) {
		if (bitmap & ((u32_t)1 << i))
			return i;
	}

	return i;
}

/*************************************************************************************
 * Function Name: get_hiprio
 * Description: Get current highest priority.
 *
 * Parameters:
 *   void
 *
 * Return:
 *   @hiprio: current highest priority
 ************************************************************************************/
static u16_t get_hiprio(void)
{
	u16_t i;
	u16_t hiprio;

	for (i = 0; i < ARRAY_SIZE(g_hiprio_bitmap); i++) {
		if (g_hiprio_bitmap[i] != 0)
			break;
	}

	hiprio = (i << 5) + get_last_bit(g_hiprio_bitmap[i]);
	return hiprio;
}

#ifdef CONFIG_PL_CFG_CHECK_STACK_OVERFLOW
/*************************************************************************************
 * Function Name: pl_check_stack_overflow
 * Description: check stack overflow whether or not.
 *
 * Parameters:
 *   @context_sp: context stack pointer.
 *   @tcb: task control block.
 *
 * Return:
 *   not zero: stack overflow.
 *   zero: not overflow.
 ************************************************************************************/
static bool pl_check_stack_overflow(void *context_sp, struct tcb *tcb)
{
	if (((*(uintptr_t *)(tcb->context_top_sp)) == CONFIG_PL_CFG_CHECK_STACK_OVERFLOW_MAGIC)
	    || context_sp < tcb->context_top_sp)
		return false;

	return true;
}
#endif /* CONFIG_PL_CFG_CHECK_STACK_OVERFLOW */

/*************************************************************************************
 * Function Name: pl_callee_save_curr_context_sp
 * Description: update context and return context_sp of the current task.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    void *context sp;
 ************************************************************************************/
void pl_callee_save_curr_context_sp(void *context_sp)
{
	struct tcb *tcb = g_task_core_blk.curr_tcb;

#ifdef CONFIG_PL_CFG_CHECK_STACK_OVERFLOW
	/* check stack overflow */
	if (pl_check_stack_overflow(context_sp, tcb))
		pl_panic_dump(PL_PANIC_REASON_STACKOVF, NULL);
#endif /* CONFIG_PL_CFG_CHECK_STACK_OVERFLOW */

	tcb->context_sp = context_sp;
}

/*************************************************************************************
 * Function Name: clear_bit_of_hiprio_bitmap
 * Description: clear the bit of highest priority table.
 *
 * Parameters:
 *   @prio: priority
 *
 * Return:
 *   void
 ************************************************************************************/
static void clear_bit_of_hiprio_bitmap(u16_t prio)
{
	u16_t idx = prio >> 5;
	u16_t offset = prio & (u16_t)0x1f;

	g_hiprio_bitmap[idx] &= ~((u32_t)1 << offset);
}

/*************************************************************************************
 * Function Name: set_bit_of_hiprio_bitmap
 * Description: set the bit of highest priority table.
 *
 * Param:
 *   @prio: priority
 *
 * Return:
 *   void
 ************************************************************************************/
static void set_bit_of_hiprio_bitmap(u16_t prio)
{
	u16_t idx = prio >> 5;
	u16_t offset = prio & (u16_t)0x1f;

	g_hiprio_bitmap[idx] |= ((u32_t)1 << offset);
}

/*************************************************************************************
 * Function Name: rdytask_list_init
 * Description:  Initialize list of ready task.
 *
 * Parameters:
 *   void
 *
 * Return:
 *   void
 ************************************************************************************/
static void rdytask_list_init(void)
{
	u16_t i;

	for (i = 0; i < CONFIG_PL_CFG_TASK_PRIORITIES_MAX + 1; i++) {
		g_task_core_blk.ready_list[i].head = NULL;
		g_task_core_blk.ready_list[i].num = 0;
	}
}

/*************************************************************************************
 * Function Name: pl_task_insert_tcb_to_rdylist
 * Description: Insert a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_insert_tcb_to_rdylist(struct tcb *tcb)
{
	u16_t prio = tcb->prio;
	struct task_list *rdylist = &g_task_core_blk.ready_list[prio];

	if (rdylist->head == NULL) {
		list_init(&tcb->node);
		rdylist->head = tcb;
	} else {
		list_add_node_at_tail(&rdylist->head->node, &tcb->node);
	}

	++rdylist->num;
	tcb->curr_state = PL_TASK_STATE_READY;
	set_bit_of_hiprio_bitmap(prio);
}

/*************************************************************************************
 * Function Name: pl_task_insert_tcb_to_delaylist
 * Description: Insert a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_insert_tcb_to_delaylist(struct tcb *tcb)
{
	struct task_list *delaylist = &g_task_core_blk.delay_list;
	struct tcb *pos = delaylist->head;

	list_for_each_entry(pos, &delaylist->head->node, struct tcb, node) {
		if (tcb->delay_ticks < pos->delay_ticks)
			break;
	}

	pos = list_prev_entry(pos, struct tcb, node);
	++delaylist->num;
	tcb->curr_state = PL_TASK_STATE_DELAY;
	list_add_node_behind(&pos->node, &tcb->node);
}

/*************************************************************************************
 * Function Name: pl_task_insert_tcb_to_exitlist
 * Description: Insert a tcb to list of exit task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_insert_tcb_to_exitlist(struct tcb *tcb)
{
	tcb->curr_state = PL_TASK_STATE_EXIT;
	list_add_node_at_tail(&g_task_core_blk.exit_list, &tcb->node);
}

/*************************************************************************************
 * Function Name: pl_task_insert_tcb_to_waitlist
 * Description: Insert a tcb to list of wait task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_insert_tcb_to_waitlist(struct list_node *wait_list, struct tcb *tcb)
{
	tcb->curr_state = PL_TASK_STATE_WAITING;
	list_add_node_at_tail(wait_list, &tcb->node);
}

/*************************************************************************************
 * Function Name: pl_task_insert_tcb_to_pendlist
 * Description: Insert a tcb to list of pending task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_insert_tcb_to_pendlist(struct tcb *tcb)
{
	tcb->curr_state = PL_TASK_STATE_PENDING;
	list_add_node_at_tail(&g_task_core_blk.pend_list, &tcb->node);
}

/*************************************************************************************
 * Function Name: pl_task_remove_tcb_from_delaylist
 * Description: remove a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_remove_tcb_from_delaylist(struct tcb *tcb)
{
	struct task_list *delaylist = &g_task_core_blk.delay_list;

	--delaylist->num;
	list_del_node(&tcb->node);
}

/*************************************************************************************
 * Function Name: pl_task_remove_tcb_from_rdylist
 * Description:  remove a tcb form list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_remove_tcb_from_rdylist(struct tcb *tcb)
{
	u16_t prio = tcb->prio;
	struct task_list *rdylist = &g_task_core_blk.ready_list[prio];
	u16_t num = rdylist->num;

	if(num == 1) {
		clear_bit_of_hiprio_bitmap(prio);
		rdylist->head = NULL;
		rdylist->num = 0;
		return;
	}

	if (tcb == rdylist->head)
		rdylist->head = list_next_entry(tcb, struct tcb, node);

	list_del_node(&tcb->node);
	--rdylist->num;
}

/*************************************************************************************
 * Function Name: pl_callee_get_next_context_sp
 * Description: update context and return context_sp of the current task.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    void *context sp;
 ************************************************************************************/
void *pl_callee_get_next_context_sp(void)
{
	u16_t hiprio;
	struct tcb *next_rdy_tcb;

	/* get highest priority task and switch to it */
	hiprio = get_hiprio();
	next_rdy_tcb = g_task_core_blk.ready_list[hiprio].head;
	g_task_core_blk.curr_tcb = next_rdy_tcb;
	return next_rdy_tcb->context_sp;
}

/*************************************************************************************
 * Function Name: pl_task_schedule_lock
 * Description: disable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_task_schedule_lock(void)
{
	pl_port_enter_critical();
	++g_task_core_blk.sched_lock_ref;
	pl_port_exit_critical();
}

/*************************************************************************************
 * Function Name: pl_task_schedule_unlock
 * Description: enable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_task_schedule_unlock(void)
{
	pl_port_enter_critical();
	--g_task_core_blk.sched_lock_ref;
	pl_port_exit_critical();
}

/*************************************************************************************
 * Function Name: pl_task_free_exit_tcb
 * Description: free exited tcb.
 *
 * Parameters:
 *   void.
 *
 * Return:
 *   void.
 ************************************************************************************/
static void pl_task_free_exit_tcb(struct pl_work *work)
{
	USED(work);
	struct tcb *pos;
	struct tcb *tmp;

	if (list_is_empty(&g_task_core_blk.exit_list))
		return;

	list_for_each_entry_safe(pos, tmp, &g_task_core_blk.exit_list, struct tcb, node) {
		list_del_node(&pos->node);
		list_init(&pos->node);
		pl_mempool_free(g_pl_default_mempool, pos);
	}
}

/*************************************************************************************
 * Function Name: pl_task_context_switch
 * Description: switch task.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 * 
 * NOTE:
 *   Do not use it in pl_port_enter_critical.
 * 
 ************************************************************************************/
void pl_task_context_switch(void)
{
	u16_t hiprio;
	struct tcb *curr_tcb;
	struct tcb *next_tcb;
	struct tcb *idle_tcb;

	if (g_task_core_blk.sched_lock_ref != 0)
		return;

	pl_port_enter_critical();
	hiprio = get_hiprio();
	curr_tcb = g_task_core_blk.curr_tcb;
	next_tcb = g_task_core_blk.ready_list[hiprio].head;

	idle_tcb = g_task_core_blk.ready_list[CONFIG_PL_CFG_TASK_PRIORITIES_MAX].head;
	if (next_tcb == idle_tcb)
		++cpu_rate_idle;

	if (curr_tcb != next_tcb)
		pl_port_switch_context();

	pl_port_exit_critical();
}

/*************************************************************************************
 * Function Name: task_entry
 * Description: routine of task entry.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   void.
 ************************************************************************************/
static void task_entry(struct tcb *tcb)
{
	struct tcb *pos;
	struct tcb *tmp;
	int exit_val = tcb->task(tcb->argc, tcb->argv);

	/* insert current task to exit list */
	pl_port_enter_critical();
	pl_task_remove_tcb_from_rdylist(tcb);
	pl_task_insert_tcb_to_exitlist(tcb);

	if (list_is_empty(&tcb->wait_head))
		goto done;

	/* recover waiting tasks */
	list_for_each_entry_safe(pos, tmp, &tcb->wait_head, struct tcb, node) {
		list_del_node(&pos->node);
		pl_task_insert_tcb_to_rdylist(pos);
		pos->wait_for_task_ret = exit_val;
	}

done:
	pl_port_exit_critical();
	pl_work_add(g_pl_sys_hiwq_handle, &g_task_core_blk.exit_free);
	/* switch task*/
	pl_task_context_switch();

	/* will be never come here */
	while(true);
}

/*************************************************************************************
 * Function Name: task_init_tcb
 * Description: initialize tcb.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @tcb: tcb of the task (must provide).
 *   @stack: stack of the task (must provide).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   none.
 ************************************************************************************/
static void task_init_tcb(const char *name, main_t task, u16_t prio,
               struct tcb *tcb, void *stack, int argc, char *argv[])
{
	tcb->name = name;
	tcb->parent = g_task_core_blk.curr_tcb;
	tcb->prio = prio;
	tcb->context_sp = stack;
	tcb->context_init_sp = stack;
	tcb->task = task;
	tcb->argc = argc;
	tcb->argv = argv;
	tcb->delay_ticks = 0;
	tcb->wait_for_task_ret = -EUNKNOWE;
	list_init(&tcb->wait_head);
	*((uintptr_t *)(tcb->context_top_sp)) = CONFIG_PL_CFG_CHECK_STACK_OVERFLOW_MAGIC;
}

/*************************************************************************************
 * Function Name: task_init_and_create
 * Description: init task and create it.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @tcb: struct tcb.
 *   @stack: stack of the task (must provide).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   void.
 ************************************************************************************/
static void task_init_and_create(const char *name, main_t task, u16_t prio,
                                 struct tcb *tcb, void *stack, size_t stack_size,
                                 int argc, char *argv[])
{
	stack = pl_port_task_stack_init(task_entry, stack, stack_size,
	                                &tcb->context_top_sp, tcb);
	task_init_tcb(name, task, prio, tcb, stack, argc, argv);

	pl_port_enter_critical();
	pl_task_insert_tcb_to_rdylist(tcb);
	pl_port_exit_critical();
	pl_task_context_switch();
}

/*************************************************************************************
 * Function Name: pl_task_sys_create_with_stack
 * Description: create a task with stack that must be provided in system.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @stack: stack of the task (must provide).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   task handle.
 ************************************************************************************/
pl_tid_t pl_task_sys_create_with_stack(const char *name, main_t task, u16_t prio,
                                       void *stack, size_t stack_size,
                                       int argc, char *argv[])
{
	struct tcb *tcb;

	/* check parameters */
	if (name == NULL || task == NULL || stack == NULL ||
	    prio > CONFIG_PL_CFG_TASK_PRIORITIES_MAX) {
		pl_early_syslog_err("param is invalid\r\n");
		return NULL;
	}

	/* alloc memory */
	tcb = (struct tcb *)pl_mempool_malloc(g_pl_default_mempool, sizeof(struct tcb));
	if (tcb == NULL) {
		pl_early_syslog_err("no mem to alloc\r\n");
		return NULL;
	}

	task_init_and_create(name, task, prio, tcb, stack, stack_size, argc, argv);
	return tcb;
}

/*************************************************************************************
 * Function Name: pl_task_create_with_stack
 * Description: create a task with stack that must be provided.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @stack: stack of the task (must provide).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   task handle.
 ************************************************************************************/
pl_tid_t pl_task_create_with_stack(const char *name, main_t task, u16_t prio,
                                   void *stack, size_t stack_size,
                                   int argc, char *argv[])
{
	struct tcb *tcb;

	if (prio < CONFIG_PL_CFG_SYS_RSVD_HIGHEST_PRIOTITY)
		prio = g_task_core_blk.curr_tcb->prio;

	tcb = pl_task_sys_create_with_stack(name, task, prio, stack, stack_size, argc, argv);
	return tcb;
}

/*************************************************************************************
 * Function Name: pl_task_sys_create
 * Description: create a task in system.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   task id.
 ************************************************************************************/
pl_tid_t pl_task_sys_create(const char *name, main_t task, u16_t prio,
                            size_t stack_size, int argc, char *argv[])
{
	void *stack;
	size_t tcb_actual_size;
	struct tcb *tcb_and_stack;

	/* check parameters */
	if (name == NULL || task == NULL || prio > CONFIG_PL_CFG_TASK_PRIORITIES_MAX) {
		pl_early_syslog_err("param is invalid\r\n");
		return NULL;
	}

	/* align address and alloc memory */
	tcb_actual_size = pl_align_size(sizeof(struct tcb), sizeof(uintptr_t) << 1);
	tcb_and_stack = pl_mempool_malloc(g_pl_default_mempool, tcb_actual_size + stack_size);
	if (tcb_and_stack == NULL)
		return NULL;

	stack = (u8_t *)tcb_and_stack + tcb_actual_size;
	task_init_and_create(name, task, prio, tcb_and_stack, stack, stack_size, argc, argv);
	return tcb_and_stack;
}

/*************************************************************************************
 * Function Name: pl_task_create
 * Description: create a task.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   task id.
 ************************************************************************************/
pl_tid_t pl_task_create(const char *name, main_t task, u16_t prio,
                        size_t stack_size, int argc, char *argv[])
{
	struct tcb *tcb_and_stack;

	if (prio < CONFIG_PL_CFG_SYS_RSVD_HIGHEST_PRIOTITY)
		prio = g_task_core_blk.curr_tcb->prio;

	tcb_and_stack = pl_task_sys_create(name, task, prio, stack_size, argc, argv);
	return tcb_and_stack;
}

/*************************************************************************************
 * Function Name: pl_task_join
 *
 * Description:
 *   wait for task exit.
 * 
 * Parameters:
 *  @tid: task id;
 *  @ret: return value of waitting task.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_join(pl_tid_t tid, int *ret)
{
	struct tcb *tcb = (struct tcb *)tid;

	if (tcb == NULL)
		return -EFAULT;

	if (tcb->curr_state == PL_TASK_STATE_EXIT)
		return -EALREADY;

	pl_port_enter_critical();
	pl_task_remove_tcb_from_rdylist(g_task_core_blk.curr_tcb);
	pl_task_insert_tcb_to_waitlist(&tcb->wait_head, g_task_core_blk.curr_tcb);
	pl_port_exit_critical();
	pl_task_context_switch();

	if (ret != NULL)
		*ret = g_task_core_blk.curr_tcb->wait_for_task_ret;

	return OK;
}

/*************************************************************************************
 * Function Name: pl_task_pend
 *
 * Description:
 *   pend task.
 * 
 * NOTE:
 *   Do not use it in pl_port_enter_critical.
 *
 * Parameters:
 *  @tid: task id, if tid is NULL, it will pend himself.
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_task_pend(pl_tid_t tid)
{
	struct tcb *tcb = (struct tcb *)tid;

	pl_port_enter_critical();
	if (tcb == NULL)
		tcb = g_task_core_blk.curr_tcb;

	if (tcb->curr_state == PL_TASK_STATE_PENDING) {
		pl_port_exit_critical();
		return;
	}

	pl_task_remove_tcb_from_rdylist(tcb);
	pl_task_insert_tcb_to_pendlist(tcb);
	pl_port_exit_critical();
	pl_task_context_switch();
}

/*************************************************************************************
 * Function Name: pl_task_resume
 *
 * Description:
 *   resume task.
 *
 * NOTE:
 *   Do not use it in pl_port_enter_critical.
 * 
 * Parameters:
 *  @tid: task id;
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_task_resume(pl_tid_t tid)
{
	struct tcb *tcb = (struct tcb *)tid;

	if (tcb == NULL)
		return;

	if (tcb->curr_state == PL_TASK_STATE_PENDING) {
		pl_port_enter_critical();
		list_del_node(&tcb->node);
		pl_task_insert_tcb_to_rdylist(tcb);
		pl_port_exit_critical();
		pl_task_context_switch();
	}
}

/*************************************************************************************
 * Function Name: Check if the specified task has exited.
 *
 * Description: specified task.
 * 
 * Parameters:
 *  @tid: The identifier of the task, of type pl_tid_t.;
 *
 * Return:
 *  true if the task has exited; otherwise, returns false.
 ************************************************************************************/
bool pl_task_is_exit(pl_tid_t tid)
{
	struct tcb *tcb = (struct tcb *)tid;

	if (tcb == NULL || tcb->curr_state != PL_TASK_STATE_EXIT)
		return false;

	return true;
}

/*************************************************************************************
 * Function Name: pl_task_kill
 *
 * Description:
 *   kill a task.
 * 
 * Parameters:
 *  @tid: task id;
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_kill(pl_tid_t tid)
{
	struct tcb *tcb = (struct tcb *)tid;

	if (tcb == NULL)
		return -EFAULT;

	if (tcb->curr_state == PL_TASK_STATE_EXIT)
		return -EALREADY;

	pl_port_enter_critical();
	pl_task_remove_tcb_from_rdylist(tcb);
	pl_task_insert_tcb_to_exitlist(tcb);
	pl_port_exit_critical();
	pl_task_context_switch();
	return OK;
}

/*************************************************************************************
 * Function Name: pl_task_delay_ticks
 *
 * Description:
 *   Delay ticks function.
 * 
 * Parameters:
 *  @ticks: delay ticks;
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
void pl_task_delay_ticks(u32_t ticks)
{
	u32_t end_ticks;

	if (ticks == 0)
		return;

	pl_port_enter_critical();
	end_ticks = g_task_core_blk.systicks + ticks;
	g_task_core_blk.curr_tcb->delay_ticks = end_ticks;
	pl_task_remove_tcb_from_rdylist(g_task_core_blk.curr_tcb);
	pl_task_insert_tcb_to_delaylist(g_task_core_blk.curr_tcb);
	pl_port_exit_critical();
	pl_task_context_switch();
}

static void update_systick(void)
{
	/* update systick */
	++g_task_core_blk.systicks;
}

/*************************************************************************************
 * Function Name: update_counter_of_cpu_rate
 *
 * Description:
 *   update counter of utilization rate.
 * 
 * Parameters:
 *  none
 *
 * Return:
 *  void.
 ************************************************************************************/
static void update_counter_of_cpu_rate(void)
{
	/* update counter of utilization rate */
	++cpu_rate_base;
	if (cpu_rate_base == CONFIG_PL_CFG_CPU_RATE_INTERVAL_TICKS) {
		g_task_core_blk.cpu_rate_base = cpu_rate_base;
		g_task_core_blk.cpu_rate_useful = cpu_rate_base -  cpu_rate_idle;
		cpu_rate_base = 0;
		cpu_rate_idle = 0;
	}
}

/*************************************************************************************
 * Function Name: update_delay_task_list
 *
 * Description:
 *   update delat task list.
 * 
 * Parameters:
 *  none
 *
 * Return:
 *  void.
 ************************************************************************************/
static void update_delay_task_list(void)
{
	struct tcb *pos;
	struct tcb *tmp;

	/* update delay list which can't be empty because of having a dummy node */
	list_for_each_entry_safe(pos, tmp, &g_task_core_blk.delay_list.head->node,
		struct tcb, node) {

		if (pos->delay_ticks > g_task_core_blk.systicks)
			break;

		pl_task_remove_tcb_from_delaylist(pos);
		pl_task_insert_tcb_to_rdylist(pos);
	}
}

/*************************************************************************************
 * Function Name: update_softtimer_list
 *
 * Description:
 *   update soft timer list.
 * 
 * Parameters:
 *  none
 *
 * Return:
 *  void.
 ************************************************************************************/
static void update_softtimer_list(void)
{
	bool list_empty;
	struct pl_stimer *pos;
	struct pl_stimer *first;
	struct pl_stimer_ctrl *timer_ctrl;

	timer_ctrl = pl_softtimer_get_ctrl();
	/* if timer list is empty, we skip it */
	list_empty = list_is_empty(&g_task_core_blk.timer_list);
	if (list_empty || timer_ctrl->daemon == NULL)
		return;

	/* Get the first entry of timer list node, We can know if the timer has timed out */
	first = list_first_entry(&g_task_core_blk.timer_list, struct pl_stimer, node);
	if (first->reach_cnt > g_task_core_blk.systicks) {
		return;
	}

	/* update soft timer list */
	list_for_each_entry(pos, &g_task_core_blk.timer_list, struct pl_stimer, node) {
		if (pos->reach_cnt > g_task_core_blk.systicks)
			break;
	}

	/* insert timer node to daemon list and wakeup the daemon task */
	list_move_chain_to_node_behind(&timer_ctrl->head, g_task_core_blk.timer_list.next,
	                                pos->node.prev);
	pl_task_resume(timer_ctrl->daemon);
}

/*************************************************************************************
 * Function Name: pl_callee_systick_expiration
 *
 * Description:
 *   The function is used to switch task, we must call it in systick handler
 *   on systick system.
 * 
 * Parameters:
 *  none
 *
 * Return:
 *  none
 ************************************************************************************/
void pl_callee_systick_expiration(void)
{
	u16_t prio;
	struct tcb *curr_tcb;
	struct task_list *rdy_list;

	/* update counter of utilization rate */
	update_counter_of_cpu_rate();

	pl_port_enter_critical();
	/* update systick */
	update_systick();
	/* update ready list */
	update_delay_task_list();
	/* update soft timer list */
	update_softtimer_list();

	/* do not to switch task when state of curr_tcb is not ready. */
	curr_tcb = g_task_core_blk.curr_tcb;
	if (curr_tcb != NULL && g_task_core_blk.sched_lock_ref == 0 &&
	    curr_tcb->curr_state == PL_TASK_STATE_READY) {
		/* round robin */
		prio = curr_tcb->prio;
		rdy_list = &g_task_core_blk.ready_list[prio];
		rdy_list->head = list_next_entry(curr_tcb, struct tcb, node);

		/* switch task */
		pl_task_context_switch();
	}
	pl_port_exit_critical();
}

/*************************************************************************************
 * Function Name: pl_task_get_syscount
 *
 * Description:
 *   The function is used to get syscount.
 *   on systick system.
 *
 * Parameters:
 *  @c: count wanted to get.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_get_syscount(u64_t *c)
{
	if (c == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	*c = g_task_core_blk.systicks;
	pl_port_exit_critical();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_task_get_cpu_rate_count
 *
 * Description:
 *   The function is used to get cpu utilization rate.
 *   on systick system.
 * 
 * Parameters:
 *  @cup_rate_base: base count of cpu rate.
 *  @rate_useful: useful count of cpu rate.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_get_cpu_rate_count(u32_t *rate_base, u32_t *rate_useful)
{
	if (rate_base == NULL || rate_useful == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	*rate_base = g_task_core_blk.cpu_rate_base;
	*rate_useful = g_task_core_blk.cpu_rate_useful;
	pl_port_exit_critical();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_task_get_cpu_rate_count
 *
 * Description:
 *   The function is used to get cpu utilization rate.
 *   on systick system.
 * 
 * Parameters:
 *  @int_part: integer part of cpu utilization rate.
 *  @deci_part: decimal part of cpu utilization rate.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_get_cpu_rate(u32_t *int_part, u32_t *deci_part)
{
	u32_t integer_part;
	u32_t decimal_part;
	u32_t _cpu_rate_base;
	u32_t _cpu_rate_useful;

	if (int_part == NULL || deci_part == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	_cpu_rate_base = g_task_core_blk.cpu_rate_base;
	_cpu_rate_useful = g_task_core_blk.cpu_rate_useful;
	pl_port_exit_critical();

	integer_part = (_cpu_rate_useful * 100) / _cpu_rate_base;
	decimal_part = (_cpu_rate_useful * 10000) / _cpu_rate_base - (integer_part * 100);

	*int_part = integer_part;
	*deci_part = decimal_part;

	return OK;
}

/*************************************************************************************
 * Function Name: pl_task_init_dummy_tcb
 * Description: initialize dummy delay and first tcb.
 *
 * Parameters:
 *   @delay_dummy_tcb: delay dummy tcb.
 *   @first_dummy_tcb: first dummy tcb.
 *
 * Return:
 *   void.
 ************************************************************************************/
static void pl_task_init_dummy_tcb(struct tcb *delay_dummy_tcb,
                                   struct tcb *first_dummy_tcb)
{
	static uintptr_t dummy_sp = CONFIG_PL_CFG_CHECK_STACK_OVERFLOW_MAGIC;

	/* init delay tcb */
	list_init(&delay_dummy_tcb->node);
	delay_dummy_tcb->delay_ticks = UINT64_MAX;
	delay_dummy_tcb->name = "delay_head";

	/* init first dummy tcb */
	list_init(&first_dummy_tcb->node);
	first_dummy_tcb->argc = 0;
	first_dummy_tcb->argv = NULL;
	first_dummy_tcb->context_top_sp = &dummy_sp;
	first_dummy_tcb->name = "first_dummy";
}

/*************************************************************************************
 * Function Name: pl_task_core_init
 * Description: initialize task component.
 *
 * Parameters:
 *   void
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_core_init(void)
{
	static struct tcb delay_dummy_tcb;
	static struct tcb first_dummy_tcb;

	cpu_rate_base = 0;
	cpu_rate_idle = 0;
	rdytask_list_init();
	list_init(&g_task_core_blk.pend_list);
	list_init(&g_task_core_blk.timer_list);
	list_init(&g_task_core_blk.exit_list);

	/* init delay_dummy_tcb and first_dummy_tcb */
	pl_task_init_dummy_tcb(&delay_dummy_tcb, &first_dummy_tcb);

	/* init task core block */
	g_task_core_blk.curr_tcb = &first_dummy_tcb;
	g_task_core_blk.systicks = 0;
	g_task_core_blk.cpu_rate_base = CONFIG_PL_CFG_CPU_RATE_INTERVAL_TICKS;
	g_task_core_blk.cpu_rate_useful = CONFIG_PL_CFG_CPU_RATE_INTERVAL_TICKS;
	g_task_core_blk.sched_lock_ref = 0;
	g_task_core_blk.delay_list.num = 0;
	g_task_core_blk.delay_list.head = &delay_dummy_tcb;

	/* init work for freeing exit tcb */
	pl_work_init(&g_task_core_blk.exit_free, pl_task_free_exit_tcb, NULL);

	pl_early_syslog_info("task core init done\r\n");
	return OK;
}
