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

#include <kernel/task.h>
#include <kernel/list.h>
#include <kernel/kernel.h>

struct rdytask_list {
	struct tcb *head;
	u16_t num;
};

/*************************************************************************************
 * Function Name: plainos_rdytask_list_init
 * Description:  Initialize list of ready task.
 ************************************************************************************/
static struct struct rdytask_list g_rdytask_list[PLAINOS_CFG_PRIORITIES_MAX + 1];

/*************************************************************************************
 * Function Name: plainos_rdytask_list_init
 * Description:  Initialize list of ready task.
 *
 * Param:
 *   void
 * Return:
 *   void
 ************************************************************************************/
static void plainos_rdytask_list_init(void)
{
	u16_t i;

	for (i = 0; i < PLAINOS_CFG_PRIORITIES_MAX + 1; i++) {
		g_rdytask_list[i].head = NULL;
		g_rdytask_list[i].num = 0;
	}
}

/*************************************************************************************
 * Function Name: plainos_insert_tcb_to_rdylist
 * Description:  Initialize list of ready task.
 *
 * Param:
 *   void
 * Return:
 *   void
 ************************************************************************************/
void  plainos_insert_tcb_to_rdylist(struct tcb *tcb)
{
	u16_t prio = tcb->prio;
	struct rdytask_list *rdylist = &g_rdytask_list[prio];

	if (rdylist.head == NULL) {
		list_init(&tcb->node);
		rdylist->head = tcb;
		rdylist->num = 1;
	} else {
		list_add_node_at_tail(&rdylist->head->node, tcb);
		++rdylist->num;
	}

	plainos_set_bit_of_hiprio_bitmap(prio);
}

/*************************************************************************************
 * Function Name: plainos_remove_tcb_from_rdylist
 * Description:  Initialize list of ready task.
 *
 * Param:
 *   void
 * Return:
 *   void
 ************************************************************************************/
void plainos_remove_tcb_from_rdylist(struct tcb *tcb)
{
	u16_t prio = tcb->prio;
	struct rdytask_list *rdylist = &g_rdytask_list[prio];
	u16_t num = rdylist->num;

	if(num == 1) {
		plainos_clear_bit_of_hiprio_bitmap(prio);
		rdylist->head = NULL;
		rdylist->num = 0;
		return;
	}

	list_del_node(&tcb->node);
	if (rdylist->head == tcb)
		rdylist->head = list_next_entry(tcb, struct tcb, node);

	--rdylist->num;
}
