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

#define LIST_HEAD(name) \
	struct list_node name = {.next = &(name), .prev = &(name)}

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
 * Return:
 *   @entry_type: next entry of specific entry.
 ************************************************************************************/
#define list_next_entry(entry, entry_type, list_node_member) \
	container_of(entry->list_node_member.next, entry_type, list_node_member)

/*************************************************************************************
 * Function Name: list_prev_entry
 * Description: Return the previous entry of specific node.
 *
 * Param:
 *   @entry: specific entry.
 * Return:
 *   @member_name: previous entry of specific entry.
 ************************************************************************************/
#define list_prev_entry(entry, entry_type, member_name) \
	container_of(entry->member_name.prev, entry_type, member_name)

/*************************************************************************************
 * Function Name: list_for_each_entry
 * Description: foreach the list inserted in a structure.
 *
 * Param:
 *   @pos: the &struct list_head to use as a loop cursor.
 *   @list_head: the head for your list.
 *   @entry_type: type of the struct.
 *   @member_name: member name of the list_node in structure.
 * Return:
 *   void
 ************************************************************************************/
#define list_for_each_entry(pos, list_head, entry_type, member_name) \
	for (pos = container_of((list_head)->next, entry_type, member_name); \
	     &pos->member_name != (list_head); \
	     pos = container_of(pos->member_name.next, entry_type, member_name))

/*************************************************************************************
 * Function Name: list_init
 * Description: inititialize a list.
 *
 * Param:
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
static void __used list_init(struct list_node *node)
{
	node->next = node;
	node->prev = node;
}

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
static void __used list_add_node_at_tail(struct list_node *head, struct list_node *node)
{
	struct list_node *tail = head->prev;

	node->prev = tail;
	node->next = head;
	tail->next = node;
	head->prev = node;
}

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
static void __used list_add_node_at_front(struct list_node *head, struct list_node *node)
{
	struct list_node *front = head->next;

	node->prev = head;
	node->next = front;
	front->prev = node;
	head->next = node;
}

/*************************************************************************************
 * Function Name: list_del_tail_node
 * Description: Delete a tail node from the list.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   void
 ************************************************************************************/
static void __used list_del_tail_node(struct list_node *head)
{
	struct list_node *tail_prev = head->prev->prev;

	tail_prev->next = head;
	head->prev = tail_prev;
}

/*************************************************************************************
 * Function Name: list_del_front_node
 * Description:  Delete a front node from the list.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   void
 ************************************************************************************/
static void __used list_del_front_node(struct list_node *head)
{
	struct list_node *front_next = head->next->next;

	front_next->prev = head;
	head->next = front_next;
}

/*************************************************************************************
 * Function Name: list_del_node
 * Description:  Delete a node from the list.
 *
 * Param:
 *   @node: the node of the list.
 * Return:
 *   void
 ************************************************************************************/
static void __used list_del_node(struct list_node *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
}

/*************************************************************************************
 * Function Name: list_is_empty
 * Description: Determine whether the list is empty.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   @bool: is empty.
 ************************************************************************************/
static bool __used list_is_empty(struct list_node *head)
{
	return head->next == head;
}

/*************************************************************************************
 * Function Name: list_next_node
 * Description: Return the next node of specific node.
 *
 * Param:
 *   @node: specific node.
 * Return:
 *   @list_node*: next node of specific node.
 ************************************************************************************/
static struct list_node* __used list_next_node(struct list_node *node)
{
	return node->next;
}

/*************************************************************************************
 * Function Name: list_prev_node
 * Description: Return the previous node of specific node.
 *
 * Param:
 *   @node: specific node.
 * Return:
 *   @list_node*: previous node of specific node.
 ************************************************************************************/
static struct list_node* __used list_prev_node(struct list_node *node)
{
	return node->prev;
}

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_LIST_H__ */
