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
#ifndef __KERNEL_LIST_H__
#define __KERNEL_LIST_H__

#include "kernel.h"
#include <stdbool.h>

/*************************************************************************************
 * @prev: previous node of the list.
 * @next: next node of the list.
 ************************************************************************************/
struct list_node {
	struct list_node *prev;
	struct list_node *next;
};

#define LIST_HEAD(name) struct list_node name = { \
		.next = &name, \
		.prev = &name  \
	}

#define LIST_INIT_VAL(name)  { \
		.prev = &name, \
		.next = &name  \
	}

/*************************************************************************************
 * Function Name: list_for_each
 * Description: foreach the list.
 *
 * Param:
 *   @pos: the &struct list_head to use as a loop cursor.
 *   @list_head: the head for your list.
 * Return:
 *   void
 ************************************************************************************/
#define list_for_each(pos, list_head) \
	for (pos = (list_head)->next; pos != (list_head); pos = pos->next)

/*************************************************************************************
 * Function Name: list_next_entry
 * Description: Return the next entry of specific node.
 *
 * Param:
 *   @entry: specific entry.
 *   @entry_type: the type of entry.
 *   @member: the name of the list_head within the struct.
 *
 * Return:
 *   @entry_type: next entry of specific entry.
 ************************************************************************************/
#define list_next_entry(entry, entry_type, member) \
	container_of(entry->member.next, entry_type, member)

/*************************************************************************************
 * Function Name: list_next_entry
 * Description: Return the next entry of specific node.
 *
 * Param:
 *   @entry: specific entry.
 *   @entry_type: the type of entry.
 *   @member: the name of the list_head within the struct.
 *
 * Return:
 *   @entry_type: next entry of specific entry.
 ************************************************************************************/
#define list_prev_entry(entry, entry_type, member) \
	container_of(entry->member.prev, entry_type, member)

/*************************************************************************************
 * Function Name: list_for_each_entry
 * Description: foreach the list inserted in a structure.
 *
 * Param:
 *   @pos: the &struct list_head to use as a loop cursor.
 *   @list_head: the head for your list.
 *   @entry_type: type of the struct.
 *   @member: member name of the list_node in structure.
 *
 * Return:
 *   void
 ************************************************************************************/
#define list_for_each_entry(pos, list_head, entry_type, member) \
	for (pos = container_of((list_head)->next, entry_type, member); \
	     &pos->member != (list_head); \
	     pos = container_of(pos->member.next, entry_type, member))

/*************************************************************************************
 * Function Name: list_for_each_entry_safe
 * Description: foreach the list inserted in a structure.
 *
 * Param:
 *   @pos: the type * to use as a loop cursor.
 *   @temp: another type * to use as temporary storage
 *   @list_head: the head for your list.
 *   @entry_type: type of the struct.
 *   @member: member name of the list_node in structure.
 *
 * Return:
 *   none
 ************************************************************************************/
#define list_for_each_entry_safe(pos, temp, list_head, entry_type, member) \
	for (pos = container_of((list_head)->next, entry_type, member), \
	     temp = list_next_entry(pos, entry_type, member); \
	     &pos->member != (list_head); \
	     pos = temp, temp = list_next_entry(temp, entry_type, member))


#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Function Name: list_init
 * Description: inititialize a list.
 *
 * Param:
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
void list_init(struct list_node *node);

/*************************************************************************************
 * Function Name: list_add_node_at_tail
 * Description: Add a node to the list tail.
 *
 * Param:
 *   @head: head node of the list.
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
void list_add_node_at_tail(struct list_node *head, struct list_node *node);
/*************************************************************************************
 * Function Name: list_add_node_at_front
 * Description: Add a node to the list front.
 *
 * Param:
 *   @head: head node of the list.
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
void list_add_node_at_front(struct list_node *head, struct list_node *node);

/*************************************************************************************
 * Function Name: list_add_node_at_front
 * Description: Add a node to the list front.
 *
 * Param:
 *   @head: head node of the list.
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
void list_add_node_behind(struct list_node *pos, struct list_node *node);

/*************************************************************************************
 * Function Name: list_del_tail_node
 * Description: Delete a tail node from the list.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   @struct list_node: tail node;
 ************************************************************************************/
struct list_node *list_del_tail_node(struct list_node *head);

/*************************************************************************************
 * Function Name: list_del_front_node
 * Description:  Delete a front node from the list.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   @struct list_node: front node;
 ************************************************************************************/
struct list_node *list_del_front_node(struct list_node *head);

/*************************************************************************************
 * Function Name: list_del_node
 * Description:  Delete a node from the list.
 *
 * Param:
 *   @node: the node of the list.
 * Return:
 *   void
 ************************************************************************************/
void list_del_node(struct list_node *node);

/*************************************************************************************
 * Function Name: list_is_empty
 * Description: Determine whether the list is empty.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   @bool: is empty.
 ************************************************************************************/
bool list_is_empty(struct list_node *head);

/*************************************************************************************
 * Function Name: list_next_node
 * Description: Return the next node of specific node.
 *
 * Param:
 *   @node: specific node.
 * Return:
 *   @list_node*: next node of specific node.
 ************************************************************************************/
struct list_node* list_next_node(struct list_node *node);

/*************************************************************************************
 * Function Name: list_prev_node
 * Description: Return the previous node of specific node.
 *
 * Param:
 *   @node: specific node.
 * Return:
 *   @list_node*: previous node of specific node.
 ************************************************************************************/
struct list_node* list_prev_node(struct list_node *node);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_LIST_H__ */
