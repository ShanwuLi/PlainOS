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
#include <lib/string.h>
#include "task_private.h"
#include "softtimer_private.h"

/*************************************************************************************
 * Description: Definitions of highest priority of task.
 ************************************************************************************/
#if (PL_CFG_PRIORITIES_MAX >= 512)
	#error "PL_CFG_PRIORITIES_MAX must less 512"
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
 *   @curr_tcb: current context tcb.
 *   @systicks: systicks.
 *   @sched_lock_ref: schedule reference counter.
 *
 ************************************************************************************/
struct task_core_blk {
	struct task_list ready_list[PL_CFG_PRIORITIES_MAX + 1];
	struct task_list delay_list;
	struct list_node pend_list;
	struct list_node timer_list;
	struct list_node exit_list;
	struct tcb *curr_tcb;
	struct count systicks;
	u32_t cpu_rate_base;
	u32_t cpu_rate_useful;
	uint_t sched_lock_ref;
};

/*************************************************************************************
 * Global Variable Name: g_hiprio_idx_tbl
 * Description: Obtain the highest priority through the priority index table.
 *              Supports up to 4096 priority levels.
 ************************************************************************************/
static u8_t __const g_hiprio_idx_tbl[256] = {
	0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x00 to 0x0F */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x10 to 0x1F */
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x20 to 0x2F */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x30 to 0x3F */
	6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x40 to 0x4F */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x50 to 0x5F */
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x60 to 0x6F */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x70 to 0x7F */
	7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x80 to 0x8F */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0x90 to 0x9F */
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0xA0 to 0xAF */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0xB0 to 0xBF */
	6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0xC0 to 0xCF */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0xD0 to 0xDF */
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  /* 0xE0 to 0xEF */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0   /* 0xF0 to 0xFF */
};

/*************************************************************************************
 * Global Variable Name: g_hiprio_bitmap
 *
 * Description: Obtain the highest priority through the priority bit map.
 *              Supports up to 4096 priority levels.
 ************************************************************************************/
static u32_t g_hiprio_bitmap[(PL_CFG_PRIORITIES_MAX + 31) / 32];

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
	u8_t i;
	u8_t last_bit;
	u8_t *p = (u8_t *)&bitmap;

	for (i = 0; i < 4; i++) {
		if (*p != 0)
			break;
		p++;
	}

	last_bit = pl_port_rodata_read8(g_hiprio_idx_tbl + (*p));
	return last_bit;
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
	g_task_core_blk.curr_tcb->context_sp = context_sp;
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

	for (i = 0; i < PL_CFG_PRIORITIES_MAX + 1; i++) {
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
		if (pl_count_cmp(&tcb->delay_ticks, &pos->delay_ticks) < 0)
			break;
	}

	pos = list_prev_entry(pos, struct tcb, node);
	++delaylist->num;
	list_add_node_behind(&pos->node, &tcb->node);
}

/*************************************************************************************
 * Function Name: pl_task_remove_tcb_from_delaylist
 * Description: Insert a tcb to list of ready task.
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
	pl_enter_critical();
	++g_task_core_blk.sched_lock_ref;
	pl_exit_critical();
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
	pl_enter_critical();
	--g_task_core_blk.sched_lock_ref;
	pl_exit_critical();
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
 ************************************************************************************/
void pl_task_context_switch(void)
{
	u16_t hiprio;
	struct tcb *curr_tcb;
	struct tcb *next_tcb;
	struct tcb *idle_tcb;

	pl_enter_critical();
	hiprio = get_hiprio();
	curr_tcb = g_task_core_blk.curr_tcb;
	next_tcb = g_task_core_blk.ready_list[hiprio].head;
	pl_exit_critical();

	idle_tcb = g_task_core_blk.ready_list[PL_CFG_PRIORITIES_MAX].head;
	if (next_tcb == idle_tcb)
		++cpu_rate_idle;

	if (curr_tcb != next_tcb)
		pl_port_switch_context();
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
	pl_enter_critical();
	tcb->curr_state = PL_TASK_STATE_EXIT;
	tcb->past_state = PL_TASK_STATE_READY;
	pl_task_remove_tcb_from_rdylist(tcb);
	list_add_node_at_tail(&g_task_core_blk.exit_list, &tcb->node);

	/* recover waiting tasks */
	list_for_each_entry_safe(pos, tmp, &tcb->wait_head, struct tcb, node) {
		list_del_node(&pos->node);
		pl_task_insert_tcb_to_rdylist(pos);
		pos->curr_state = PL_TASK_STATE_READY;
		pos->past_state = PL_TASK_STATE_WAITING;
		pos->wait_for_task_ret = exit_val;
	}

	pl_exit_critical();
	/* switch task*/
	pl_task_context_switch();
	/* will be never come here */
	while(1);
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
	tcb->prio = (prio > 0) ? prio : tcb->parent->prio;
	tcb->context_sp = stack;
	tcb->task = task;
	tcb->argc = argc;
	tcb->argv = argv;
	tcb->signal = 0;
	tcb->curr_state = PL_TASK_STATE_READY;
	tcb->past_state = PL_TASK_STATE_EXIT;
	tcb->delay_ticks.hi32 = 0;
	tcb->delay_ticks.lo32 = 0;
	tcb->wait_for_task_ret = ERROR;
	list_init(&tcb->wait_head);
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
	stack = pl_port_task_stack_init(task_entry, stack, stack_size, tcb);
	task_init_tcb(name, task, prio, tcb, stack, argc, argv);

	pl_enter_critical();
	pl_task_insert_tcb_to_rdylist(tcb);
	pl_exit_critical();
	pl_task_context_switch();
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
tid_t pl_task_create_with_stack(const char *name, main_t task, u16_t prio,
                                void *stack, size_t stack_size,
                                int argc, char *argv[])
{
	struct tcb *tcb;

	if (task == NULL || stack == NULL) {
		pl_early_syslog_err("task or stack is NULL\n");
		return (tid_t)ERR_TO_PTR(-EFAULT);
	}

	tcb = (struct tcb *)pl_mempool_malloc(g_pl_default_mempool, sizeof(struct tcb));
	if (tcb == NULL) {
		pl_early_syslog_err("no mem to alloc\r\n");
		return (tid_t)ERR_TO_PTR(-ENOMEM);
	}

	task_init_and_create(name, task, prio, tcb, stack, stack_size, argc, argv);
	return tcb;
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
tid_t pl_task_create(const char *name, main_t task, u16_t prio,
                     size_t stack_size, int argc, char *argv[])
{
	void *stack;
	struct tcb *tcb_and_stack;
	size_t tcb_actual_size = pl_align_size(sizeof(struct tcb), sizeof(uintptr_t) << 1);

	tcb_and_stack = pl_mempool_malloc(g_pl_default_mempool, tcb_actual_size + stack_size);
	if (tcb_and_stack == NULL)
		return ERR_TO_PTR(-ENOMEM);

	stack = (u8_t *)tcb_and_stack + tcb_actual_size;
	task_init_and_create(name, task, prio, tcb_and_stack, stack, stack_size, argc, argv);
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
int pl_task_join(tid_t tid, int *ret)
{
	struct tcb *tcb = (struct tcb *)tid;

	if (tcb == NULL)
		return -EFAULT;

	if (tcb->curr_state == PL_TASK_STATE_EXIT)
		return ERROR;

	pl_enter_critical();
	g_task_core_blk.curr_tcb->curr_state = PL_TASK_STATE_WAITING;
	g_task_core_blk.curr_tcb->past_state = PL_TASK_STATE_READY;

	pl_task_remove_tcb_from_rdylist(g_task_core_blk.curr_tcb);
	list_add_node_at_tail(&tcb->wait_head, &g_task_core_blk.curr_tcb->node);
	pl_exit_critical();
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
 * Parameters:
 *  @tid: task id;
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_pend(tid_t tid)
{
	struct tcb *tcb = (struct tcb *)tid;

	if (tcb == NULL)
		return -EFAULT;

	pl_enter_critical();
	pl_task_remove_tcb_from_rdylist(tcb);
	list_add_node_at_tail(&g_task_core_blk.pend_list, &tcb->node);
	tcb->curr_state = PL_TASK_STATE_PENDING;
	tcb->past_state = PL_TASK_STATE_READY;
	pl_exit_critical();
	pl_task_context_switch();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_task_resume
 *
 * Description:
 *   resume task.
 * 
 * Parameters:
 *  @tid: task id;
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_resume(tid_t tid)
{
	struct tcb *tcb = (struct tcb *)tid;

	if (tcb == NULL)
		return -EFAULT;

	pl_enter_critical();
	list_del_node(&tcb->node);
	pl_task_insert_tcb_to_rdylist(tcb);
	tcb->curr_state = PL_TASK_STATE_READY;
	tcb->past_state = PL_TASK_STATE_PENDING;
	pl_exit_critical();
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
	u32_t end_ticks_lo32;
	u32_t end_ticks_hi32;

	if (ticks == 0)
		return;

	pl_enter_critical();
	end_ticks_lo32 = g_task_core_blk.systicks.lo32;
	end_ticks_hi32 = g_task_core_blk.systicks.hi32;

	ticks += end_ticks_lo32;
	if (ticks < end_ticks_lo32)
		++end_ticks_hi32;

	g_task_core_blk.curr_tcb->delay_ticks.hi32 = end_ticks_hi32;
	g_task_core_blk.curr_tcb->delay_ticks.lo32 = ticks;
	g_task_core_blk.curr_tcb->curr_state = PL_TASK_STATE_DELAY;
	g_task_core_blk.curr_tcb->past_state = PL_TASK_STATE_READY;

	pl_task_remove_tcb_from_rdylist(g_task_core_blk.curr_tcb);
	pl_task_insert_tcb_to_delaylist(g_task_core_blk.curr_tcb);
	pl_exit_critical();
	pl_task_context_switch();
}

static void update_systick(void)
{
	/* update systick */
	++g_task_core_blk.systicks.lo32;
	if (g_task_core_blk.systicks.lo32 == 0)
		++g_task_core_blk.systicks.hi32;
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
	if (cpu_rate_base == PL_CFG_CPU_RATE_INTERVAL_TICKS) {
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

	/* update delay list */
	list_for_each_entry_safe(pos, tmp, &g_task_core_blk.delay_list.head->node,
		struct tcb, node) {

		if (pl_count_cmp(&pos->delay_ticks, &g_task_core_blk.systicks) > 0)
			break;

		pl_task_remove_tcb_from_delaylist(pos);
		pl_task_insert_tcb_to_rdylist(pos);
		pos->curr_state = PL_TASK_STATE_READY;
		pos->past_state = PL_TASK_STATE_DELAY;
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
	struct softtimer *pos;
	struct softtimer *tmp;
	struct softtimer *first;
	struct softtimer_ctrl *timer_ctrl;

	timer_ctrl = pl_softtimer_get_ctrl();
	/* if timer list is empty, we skip it */
	list_empty = list_is_empty(&g_task_core_blk.timer_list);
	if (list_empty || timer_ctrl->daemon == NULL)
		return;

	first = list_first_entry(&g_task_core_blk.timer_list, struct softtimer, node);
	if (pl_count_cmp(&first->reach_cnt, &g_task_core_blk.systicks) > 0) {
		return;
	}

	/* update soft timer list */
	list_for_each_entry_safe(pos, tmp, &g_task_core_blk.timer_list,
	    struct softtimer, node) {

		if (pl_count_cmp(&pos->reach_cnt, &g_task_core_blk.systicks) > 0)
			break;

		list_del_node(&pos->node);
		list_add_node_at_tail(&timer_ctrl->head, &pos->node);
	}

	pl_task_insert_tcb_to_rdylist((struct tcb *)(timer_ctrl->daemon));
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

	/* update systick */
	update_systick();
	/* update ready list */
	update_delay_task_list();
	/* update soft timer list */
	update_softtimer_list();
	/* update counter of utilization rate */
	update_counter_of_cpu_rate();

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
int pl_task_get_syscount(struct count *c)
{
	if (c == NULL)
		return -EFAULT;

	pl_enter_critical();
	c->hi32 = g_task_core_blk.systicks.hi32;
	c->lo32 = g_task_core_blk.systicks.lo32;
	pl_exit_critical();

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

	*rate_base = g_task_core_blk.cpu_rate_base;
	*rate_useful = g_task_core_blk.cpu_rate_useful;
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

	pl_enter_critical();
	_cpu_rate_base = g_task_core_blk.cpu_rate_base;
	_cpu_rate_useful = g_task_core_blk.cpu_rate_useful;
	pl_exit_critical();

	integer_part = (_cpu_rate_useful * 100) / _cpu_rate_base;
	decimal_part = (_cpu_rate_useful * 10000) / _cpu_rate_base - (integer_part * 100);

	*int_part = integer_part;
	*deci_part = decimal_part;

	return OK;
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

	cpu_rate_base = 0;
	cpu_rate_idle = 0;
	rdytask_list_init();
	list_init(&g_task_core_blk.pend_list);
	list_init(&g_task_core_blk.timer_list);
	list_init(&g_task_core_blk.exit_list);

	list_init(&delay_dummy_tcb.node);
	delay_dummy_tcb.delay_ticks.hi32 = UINT32_MAX;
	delay_dummy_tcb.delay_ticks.lo32 = UINT32_MAX;
	delay_dummy_tcb.name = "delay_head";

	g_task_core_blk.curr_tcb = NULL;
	g_task_core_blk.systicks.hi32 = 0;
	g_task_core_blk.systicks.lo32 = 0;
	g_task_core_blk.cpu_rate_base = PL_CFG_CPU_RATE_INTERVAL_TICKS;
	g_task_core_blk.cpu_rate_useful = PL_CFG_CPU_RATE_INTERVAL_TICKS;
	g_task_core_blk.sched_lock_ref = 0;
	g_task_core_blk.delay_list.num = 0;
	g_task_core_blk.delay_list.head = &delay_dummy_tcb;

	pl_early_syslog_info("task core init successfully\r\n");
	return OK;
}
