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

#define TASK_TCB_MAGIC                         0xDeadBeef
/*************************************************************************************
 * Description: Definitions of highest priority of task.
 ************************************************************************************/
#if (PL_CFG_PRIORITIES_MAX >= 4096)
	#error PL_CFG_PRIORITIES_MAX must less 4096
#endif

#define HIPRIO_BITMAP_SIZE                      ((PL_CFG_PRIORITIES_MAX + 7) / 8)
#define HIPRIO_LEV1_BITMAP_SIZE                 ((PL_CFG_PRIORITIES_MAX + 63) / 64)
#define HIPRIO_LEV2_BITMAP_SIZE                 ((PL_CFG_PRIORITIES_MAX + 511) / 512)
#define HIPRIO_OF(lv2_idx, lv1_idx, idx, bit)   ( ((lv2_idx) << 9) \
                                                | ((lv1_idx) << 6) \
                                                | ((idx) << 3) \
                                                | ((bit) << 0));

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
	struct tcb idle_task;
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
static u8_t g_hiprio_bitmap[HIPRIO_BITMAP_SIZE];
static u8_t g_hiprio_bitmap_lv1[HIPRIO_LEV1_BITMAP_SIZE];
static u8_t g_hiprio_bitmap_lv2[HIPRIO_LEV2_BITMAP_SIZE];
static u8_t g_hiprio_bitmap_lv3;

/*************************************************************************************
 * Global Variable Name: g_task_core_blk
 * Description:  task core block.
 ************************************************************************************/
static struct task_core_blk g_task_core_blk;
static u32_t g_idle_task_stack[256];
static u32_t g_idle1_task_stack[256];

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
	u16_t hiprio;
	u8_t bitmap_bit;
	u8_t bitmap_index;
	u8_t bitmap_lv1_index;
	u8_t bitmap_lv2_index;

	bitmap_lv2_index = g_hiprio_idx_tbl[g_hiprio_bitmap_lv3];
	hiprio           = HIPRIO_OF(bitmap_lv2_index, 0, 0, 0);

	bitmap_lv1_index = g_hiprio_idx_tbl[g_hiprio_bitmap_lv2[hiprio]];
	hiprio          += HIPRIO_OF(0, bitmap_lv1_index, 0, 0);

	bitmap_index     = g_hiprio_idx_tbl[g_hiprio_bitmap_lv1[hiprio]];
	hiprio          += HIPRIO_OF(0, 0, bitmap_index, 0);

	bitmap_bit       = g_hiprio_idx_tbl[g_hiprio_bitmap[bitmap_index]];
	hiprio          += HIPRIO_OF(0, 0, 0, bitmap_bit);

	return hiprio;
}

/*************************************************************************************
 * Function Name: pl_switch_to_hiprio_task
 * Description: Switch to the task of highest priority.
 *
 * Parameters:
 *   void
 *
 * Return:
 *   void
 ************************************************************************************/
static struct tcb *get_next_rdy_tcb(void)
{
	struct tcb *next_tcb;
	u16_t hiprio = get_hiprio();
	struct tcb *curr_tcb = g_task_core_blk.curr_tcb;

	if (curr_tcb != NULL && hiprio >= curr_tcb->prio)
		next_tcb = list_next_entry(curr_tcb, struct tcb, node);
	else
		next_tcb = g_task_core_blk.ready_list[hiprio].head;

	return next_tcb;
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
 * Function Name: pl_callee_update_context
 * Description: update context and return context_sp of the current task.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    void *context sp;
 ************************************************************************************/
void *pl_callee_update_context(void)
{
	struct tcb *next_tcb = get_next_rdy_tcb();
	g_task_core_blk.curr_tcb = next_tcb;

	//pl_early_syslog_info("pl_callee_update_context, sp:0x%x\r\n", next_tcb->context_sp);
	return next_tcb->context_sp;
}

/*************************************************************************************
 * Function Name: pl_get_curr_tcb
 * Description: get current tcb.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    struct tcb *;
 ************************************************************************************/
struct tcb *pl_get_curr_tcb(void)
{
	return g_task_core_blk.curr_tcb;
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
	g_hiprio_bitmap[prio >> 3] &= (u8_t)(~(1 << (prio & 0x7)));
	if (g_hiprio_bitmap[prio >> 3] != 0)
		return;

	g_hiprio_bitmap_lv1[prio >> 6] &= (u8_t)(~(1 << ((prio & 0x3f) >> 3)));
	if (g_hiprio_bitmap_lv1[prio >> 6] != 0)
		return;

	g_hiprio_bitmap_lv2[prio >> 9] &= (u8_t)(~(1 << ((prio & 0x1ff) >> 6)));
	if (g_hiprio_bitmap_lv2[prio >> 9] != 0)
		return;

	g_hiprio_bitmap_lv3 &= (u8_t)(~(1 << ((prio & 0x1000) >> 9)));
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
	g_hiprio_bitmap[prio >> 3]     |= (u8_t)(1 << ((prio & 0x7) >> 0));
	g_hiprio_bitmap_lv1[prio >> 6] |= (u8_t)(1 << ((prio & 0x3f) >> 3));
	g_hiprio_bitmap_lv2[prio >> 9] |= (u8_t)(1 << ((prio & 0x1ff) >> 6));
	g_hiprio_bitmap_lv3            |= (u8_t)(1 << ((prio & 0x1000) >> 9));
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
 * Function Name: remove_tcb_from_rdylist
 * Description:  remove a tcb form list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
static void __used remove_tcb_from_rdylist(struct tcb *tcb)
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

	list_del_node(&tcb->node);
	if (rdylist->head == tcb)
		rdylist->head = list_next_entry(tcb, struct tcb, node);

	--rdylist->num;
}

/*************************************************************************************
 * Function Name: pl_enable_schedule
 * Description: enable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_enable_schedule(void)
{
	g_task_core_blk.sched_enable = true;
}

/*************************************************************************************
 * Function Name: pl_disable_schedule
 * Description: disable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_disable_schedule(void)
{
	g_task_core_blk.sched_enable = false;
}

/*************************************************************************************
 * Function Name: pl_switch_to_next_same_prio_task
 * Description: Switch to the next task of same priority.
 *
 * Parameters:
 *   void
 *
 * Return:
 *   void
 ************************************************************************************/
void pl_switch_to_next_same_prio_task(void);
void pl_switch_to_next_same_prio_task(void)
{
	struct tcb *curr_tcb = g_task_core_blk.curr_tcb;

	g_task_core_blk.curr_tcb = list_next_entry(curr_tcb, struct tcb, node);
	pl_port_task_switch();
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
	tcb->magic = TASK_TCB_MAGIC;
	tcb->task = task;

	irqstate = pl_port_irq_save();
	insert_tcb_to_rdylist(tcb);
	pl_port_irq_restore(irqstate);
	pl_enable_schedule();
	//pl_port_task_switch();

	return tcb;
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
	/* update systick */
	++g_task_core_blk.systicks.lo32;
	if (g_task_core_blk.systicks.lo32 == 0)
		++g_task_core_blk.systicks.hi32;

	//pl_early_syslog_info("systick:%d\r\n", g_task_core_blk.systicks.lo32);
	pl_port_task_switch();
}

extern int RTS_PORT_SystickInit(void);
static int idle_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	RTS_PORT_SystickInit();

	while(1) {
		pl_early_syslog_info("idle task++++++++++++++++++++++\r\n");
		for (volatile int i = 0; i < 2000000; i++)
			;
		pl_early_syslog_info("systick:%d\r\n", g_task_core_blk.systicks.lo32);

		for (int j = 0; j < g_task_core_blk.ready_list[10].num; j++)
			pl_early_syslog_info("node[%d]\r\n", j);
	}

	return 0;
}

static int idle1_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	//RTS_PORT_SystickInit();

	while(1) {
		pl_early_syslog_info("idle task==========\r\n");
		pl_early_syslog_info("systick:%d\r\n", g_task_core_blk.systicks.lo32);
		for (volatile int i = 0; i < 2000000; i++)
			;

		struct tcb *tcb = g_task_core_blk.ready_list[10].head;
		for (int j=0; j < g_task_core_blk.ready_list[10].num;j++) {
			pl_early_syslog_info("tcb[%d].name:%s\r\n", j, tcb->name);
			tcb = list_next_entry(tcb, struct tcb, node);
		}
	}

	return 0;
}

struct tcb g_idle1_task;
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
	rdytask_list_init();
	g_task_core_blk.curr_tcb = NULL;
	g_task_core_blk.systicks.hi32 = 0;
	g_task_core_blk.systicks.lo32 = 0;
	g_task_core_blk.sched_enable = false;


	pl_task_create_with_stack("fegeg", idle_task, 10, &g_task_core_blk.idle_task,
	                           g_idle_task_stack, 256, 0, NULL);
	pl_task_create_with_stack("ewrett", idle1_task, 10, &g_idle1_task,
	                           g_idle1_task_stack, 256, 0, NULL);
	pl_early_syslog_info("task core init successfully\r\n");

	return OK;
}
core_initcall(pl_task_core_blk_init);
