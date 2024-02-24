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

#include <errno.h>
#include <pl_cfg.h>
#include <kernel/task.h>
#include <kernel/list.h>
#include <kernel/kernel.h>
#include <pl_port.h>
#include <kernel/initcall.h>
#include <kernel/syslog.h>
#include <kernel/initcall.h>

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
 *   @sched_enable: schedule state, true: enable schedule, false: disable schedule.
 *
 ************************************************************************************/
struct task_core_blk {
	struct task_list ready_list[PL_CFG_PRIORITIES_MAX + 1];
	struct task_list pend_list;
	struct task_list delay_list;
	struct tcb *curr_tcb;
	struct count systicks;
	bool sched_enable;
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
 * Global Variable Name: g_hiprio_bitmap, g_hiprio_bitmap_lv1, g_hiprio_bitmap_lv2,
 *                       g_hiprio_bitmap_lv3
 *
 * Description: Obtain the highest priority through the priority index table.
 *              Supports up to 4096 priority levels.
 ************************************************************************************/
static u32_t g_hiprio_bitmap[(PL_CFG_PRIORITIES_MAX + 31) / 32];

/*************************************************************************************
 * Global Variable Name: g_task_core_blk
 * Description:  task core block.
 ************************************************************************************/
struct task_core_blk g_task_core_blk;

/*************************************************************************************
 * Function Name: get_lead_zero
 * Description: Get leading zero of bitmap.
 *
 * Parameters:
 *   void
 *
 * Return:
 *   @leadzero: leadzero
 ************************************************************************************/
static u8_t get_lead_zero(u32_t bitmap)
{
	u8_t i;
	u8_t lead_zero;
	u8_t *p = (u8_t *)&bitmap;

	for (i = 0; i < 4; i++) {
		if (*p != 0)
			break;
		p++;
	}

	lead_zero = pl_port_rodata_read8(g_hiprio_idx_tbl + (*p));
	return lead_zero;
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

	hiprio = (i << 5) + get_lead_zero(g_hiprio_bitmap[i]);
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
 * Function Name: insert_tcb_to_rdylist
 * Description: Insert a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
static void insert_tcb_to_rdylist(struct tcb *tcb)
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
 * Function Name: insert_tcb_to_delaylist
 * Description: Insert a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
static void insert_tcb_to_delaylist(struct tcb *tcb)
{
	struct task_list *delaylist = &g_task_core_blk.delay_list;
	struct tcb *pos = delaylist->head;

	list_for_each_entry(pos, &delaylist->head->node, struct tcb, node) {
		if (count_cmp(&tcb->delay_ticks, &pos->delay_ticks) < 0)
			break;
	}

	pos = list_prev_entry(pos, struct tcb, node);
	++delaylist->num;
	list_add_node_behind(&pos->node, &tcb->node);
}

/*************************************************************************************
 * Function Name: remove_tcb_from_delaylist
 * Description: Insert a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
static void remove_tcb_from_delaylist(struct tcb *tcb)
{
	struct task_list *delaylist = &g_task_core_blk.delay_list;

	--delaylist->num;
	list_del_node(&tcb->node);
}


/*************************************************************************************
 * Function Name: remove_tcb_from_rdylist
 * Description:  remove a tcb form list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
static void remove_tcb_from_rdylist(struct tcb *tcb)
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
 * Function Name: pl_callee_get_next_context
 * Description: update context and return context_sp of the current task.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    void *context sp;
 ************************************************************************************/
void *pl_callee_get_next_context(void)
{
	u16_t hiprio = get_hiprio();
	struct tcb *next_rdy_tcb = g_task_core_blk.ready_list[hiprio].head;

	g_task_core_blk.curr_tcb = next_rdy_tcb;
	return next_rdy_tcb->context_sp;
}

/*************************************************************************************
 * Function Name: pl_schedule_unlock
 * Description: enable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_schedule_unlock(void)
{
	irqstate_t irqstate;

	irqstate = pl_port_irq_save();
	g_task_core_blk.sched_enable = true;
	pl_port_irq_restore(irqstate);
}

/*************************************************************************************
 * Function Name: pl_schedule_lock
 * Description: disable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_schedule_lock(void)
{
	irqstate_t irqstate;

	irqstate = pl_port_irq_save();
	g_task_core_blk.sched_enable = false;
	pl_port_irq_restore(irqstate);
}

/*************************************************************************************
 * Function Name: pl_context_switch
 * Description: switch task.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_context_switch(void)
{
	u16_t hiprio;
	irqstate_t irqstate;
	struct tcb *curr_tcb;
	struct tcb *next_tcb;
	
	irqstate = pl_port_irq_save();
	hiprio = get_hiprio();
	curr_tcb = g_task_core_blk.curr_tcb;
	next_tcb = g_task_core_blk.ready_list[hiprio].head;
	pl_port_irq_restore(irqstate);

	if (curr_tcb != next_tcb) {
		pl_early_syslog_info("switch\r\n");
		pl_port_switch_context();
	}
}

/*************************************************************************************
 * Function Name: pl_task_create_with_stack
 * Description: create a task with stack that must be provided.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @tcb: tcb of the task (must provide).
 *   @stack: stack of the task (must provide).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   task handle.
 ************************************************************************************/
tid_t pl_task_create_with_stack(const char *name, task_t task, u16_t prio,
                                struct tcb *tcb, void *stack, size_t stack_size,
                                int argc, char *argv[])
{
	irqstate_t irqstate;

	if (task == NULL || tcb == NULL || stack == NULL) {
		pl_early_syslog_err("task, tcb or stack is NULL\n");
		return (tid_t)ERR_TO_PTR(-EFAULT);
	}

	stack = pl_port_task_stack_init(task, stack, stack_size, argc, argv);

	tcb->name = name;
	tcb->parent = g_task_core_blk.curr_tcb;
	tcb->signal = 0;
	tcb->prio = (prio > 0) ? prio : tcb->parent->prio;
	tcb->curr_state = PL_TASK_STATE_READY;
	tcb->past_state = PL_TASK_STATE_EXIT;
	tcb->context_sp = stack;
	tcb->task = task;
	tcb->delay_ticks.hi32 = 0;
	tcb->delay_ticks.lo32 = 0;

	irqstate = pl_port_irq_save();
	insert_tcb_to_rdylist(tcb);
	pl_port_irq_restore(irqstate);
	pl_context_switch();

	return tcb;
}

/*************************************************************************************
 * Function Name: pl_delay_ticks
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
void pl_delay_ticks(u32_t ticks)
{
	irqstate_t irqstate;
	u32_t end_ticks_lo32;
	u32_t end_ticks_hi32;

	if (ticks == 0)
		return;

	irqstate = pl_port_irq_save();
	end_ticks_lo32 = g_task_core_blk.systicks.lo32;
	end_ticks_hi32 = g_task_core_blk.systicks.hi32;

	ticks += end_ticks_lo32;
	if (ticks < end_ticks_lo32)
		++end_ticks_hi32;

	g_task_core_blk.curr_tcb->delay_ticks.hi32 = end_ticks_hi32;
	g_task_core_blk.curr_tcb->delay_ticks.lo32 = ticks;
	g_task_core_blk.curr_tcb->curr_state = PL_TASK_STATE_DELAY;
	g_task_core_blk.curr_tcb->past_state = PL_TASK_STATE_READY;
	remove_tcb_from_rdylist(g_task_core_blk.curr_tcb);
	insert_tcb_to_delaylist(g_task_core_blk.curr_tcb);
	pl_port_irq_restore(irqstate);
	pl_context_switch();
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
	struct tcb *pos;
	struct tcb *tmp;
	struct tcb *curr_tcb = g_task_core_blk.curr_tcb;

	/* update systick */
	++g_task_core_blk.systicks.lo32;
	if (g_task_core_blk.systicks.lo32 == 0)
		++g_task_core_blk.systicks.hi32;

	//pl_early_syslog_info("prio:%d\r\n", g_task_core_blk.curr_tcb->prio);
	prio = curr_tcb->prio;
	g_task_core_blk.ready_list[prio].head = list_next_entry(curr_tcb, struct tcb, node);

	list_for_each_entry_safe(pos, tmp, &g_task_core_blk.delay_list.head->node,
		struct tcb, node) {
		if (count_cmp(&pos->delay_ticks, &g_task_core_blk.systicks) <= 0) {
			remove_tcb_from_delaylist(pos);
			insert_tcb_to_rdylist(pos);
			pos->curr_state = PL_TASK_STATE_READY;
			pos->past_state = PL_TASK_STATE_DELAY;
		} else {
			break;
		}
	}

	if (g_task_core_blk.sched_enable)
		pl_context_switch();
}

/*************************************************************************************
 * Function Name: pl_task_core_blk_init
 * Description: initialize task core block.
 *
 * Parameters:
 *   void
 *
 * Return:
 *   none
 ************************************************************************************/
static int pl_task_core_blk_init(void)
{
	static struct tcb delay_dummy_tcb;

	rdytask_list_init();
	list_init(&delay_dummy_tcb.node);
	delay_dummy_tcb.delay_ticks.hi32 = UINT32_MAX;
	delay_dummy_tcb.delay_ticks.lo32 = UINT32_MAX;
	delay_dummy_tcb.name = "delay_dummy";

	g_task_core_blk.curr_tcb = NULL;
	g_task_core_blk.systicks.hi32 = 0;
	g_task_core_blk.systicks.lo32 = 0;
	g_task_core_blk.sched_enable = true;
	g_task_core_blk.delay_list.num = 0;
	g_task_core_blk.delay_list.head = &delay_dummy_tcb;

	pl_early_syslog_info("task core init successfully\r\n");
	return OK;
}
core_initcall(pl_task_core_blk_init);
