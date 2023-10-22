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
#include <config.h>
#include <kernel/task.h>
#include <kernel/list.h>
#include <kernel/kernel.h>
#include <plainos_port.h>

/*************************************************************************************
 * Description: Definitions for highest priority of task.
 ************************************************************************************/
#define HIPRIO_BITMAP_SIZE                      ((PLAINOS_CFG_PRIORITIES_MAX + 7) / 8)
#define HIPRIO_LEV1_BITMAP_SIZE                 ((PLAINOS_CFG_PRIORITIES_MAX + 63) / 64)
#define HIPRIO_LEV2_BITMAP_SIZE                 ((PLAINOS_CFG_PRIORITIES_MAX + 511) / 512)
#define HIPRIO_OF(lv2_idx, lv1_idx, idx, bit)   ( ((lv2_idx) << 9) \
                                                | ((lv1_idx) << 6) \
                                                | ((idx) << 3) \
                                                | ((bit) << 0));

/*************************************************************************************
 * structure Name: rdytask_list
 * Description: Define a list of ready task.
 ************************************************************************************/
struct rdytask_list {
	struct tcb *head;
	u16_t num;
};

/*************************************************************************************
 * Global Variable Name: g_hiprio_idx_tbl
 * Description: Obtain the highest priority through the priority index table.
 *              Supports up to 4096 priority levels.
 ************************************************************************************/
static const u8_t g_hiprio_idx_tbl[256] = {
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
 * Global Variable Name: g_rdytask_list
 * Description:  List of ready task.
 ************************************************************************************/
struct tcb *g_plainos_curr_task_tcb;

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
 * Global Variable Name: g_rdytask_list
 * Description:  List of ready task.
 ************************************************************************************/
static struct rdytask_list g_rdytask_list[PLAINOS_CFG_PRIORITIES_MAX + 1];

/*************************************************************************************
 * Function Name: get_hiprio
 * Description: Get current highest priority.
 *
 * Param:
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
 * Function Name: clear_bit_of_hiprio_bitmap
 * Description: clear the bit of highest priority table.
 *
 * Param:
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
 * Param:
 *   void
 * Return:
 *   void
 ************************************************************************************/
static void rdytask_list_init(void)
{
	u16_t i;

	for (i = 0; i < PLAINOS_CFG_PRIORITIES_MAX + 1; i++) {
		g_rdytask_list[i].head = NULL;
		g_rdytask_list[i].num = 0;
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
	struct rdytask_list *rdylist = &g_rdytask_list[prio];

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
static void remove_tcb_from_rdylist(struct tcb *tcb)
{
	u16_t prio = tcb->prio;
	struct rdytask_list *rdylist = &g_rdytask_list[prio];
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
 * Function Name: plainos_switch_to_next_same_prio_task
 * Description: Switch to the next task of same priority.
 *
 * Param:
 *   void
 * Return:
 *   void
 ************************************************************************************/
void plainos_switch_to_next_same_prio_task(void)
{
	g_plainos_curr_task_tcb = list_next_entry(g_plainos_curr_task_tcb, struct tcb, node);
	plainos_port_schedule();
}

/*************************************************************************************
 * Function Name: plainos_switch_to_next_same_prio_task
 * Description: Switch to the task of highest priority.
 *
 * Param:
 *   void
 * Return:
 *   void
 ************************************************************************************/
void plainos_switch_to_hiprio_task(void)
{
	u16_t hiprio = get_hiprio();

	if (hiprio >= g_plainos_curr_task_tcb->prio) {
		g_plainos_curr_task_tcb = list_next_entry(g_plainos_curr_task_tcb, struct tcb, node);
	} else {
		g_plainos_curr_task_tcb = g_rdytask_list[hiprio].head;
	}

	/* OS schedule */
	plainos_port_schedule();
}
