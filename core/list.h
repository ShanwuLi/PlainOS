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
#ifndef __LIST_H__
#define __LIST_H__

#include "core.h"

/*************************************************************************************
 * @prev: previous node of the list.
 * @next: next node of the list.
 ************************************************************************************/
struct list_node {
	struct list_node *prev;
	struct list_node *next;
};

#define LIST_NODE(name) \
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
 * Function Name: list_for_each_entry
 * Description: foreach the list inserted in a structure.
 *
 * Param:
 *   @pos: the &struct list_head to use as a loop cursor.
 *   @list_head: the head for your list.
 *   @struct_type: type of the struct.
 *   @list_node_member: member name of the list_node in structure.
 * Return:
 *   void
 ************************************************************************************/
#define list_for_each_entry(pos, list_head, struct_type, list_node_member) \
	for (pos = container_of((list_head)->next, struct_type, list_node_member); \
	     &pos->member != (list_head); \
	     pos = container_of(pos->list_node_member.next, struct_type, list_node_member))


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
 * Function Name: list_add_tail
 * Description: Add a node to the list tail.
 *
 * Param:
 *   @head: head node of the list.
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
void list_add_tail(struct list_node *head, struct list_node *node);

/*************************************************************************************
 * Function Name: list_add_front
 * Description: Add a node to the list front.
 *
 * Param:
 *   @head: head node of the list.
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
void list_add_front(struct list_node *head, struct list_node *node);

/*************************************************************************************
 * Function Name: list_del_tail
 * Description: Delete a tail node from the list.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   void
 ************************************************************************************/
void list_del_tail(struct list_node *head);

/*************************************************************************************
 * Function Name: list_del_front
 * Description:  Delete a front node from the list.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   void
 ************************************************************************************/
void list_del_front(struct list_node *head);




#ifdef __cplusplus
}
#endif

#endif /* __LIST_H__ */