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
#include "kernel/kernel.h"
#include <stdbool.h>
#include <kernel/list.h>

/*************************************************************************************
 * Function Name: list_init
 * Description: inititialize a list.
 *
 * Param:
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
void list_init(struct list_node *node)
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
void list_add_node_at_tail(struct list_node *head, struct list_node *node)
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
void list_add_node_at_front(struct list_node *head, struct list_node *node)
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
void list_del_tail_node(struct list_node *head)
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
void list_del_front_node(struct list_node *head)
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
void list_del_node(struct list_node *node)
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
bool list_is_empty(struct list_node *head)
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
struct list_node* list_next_node(struct list_node *node)
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
struct list_node* list_prev_node(struct list_node *node)
{
	return node->prev;
}

