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
 * Function Name: list_add_tail
 * Description: Add a node to the list tail.
 *
 * Param:
 *   @head: head node of the list.
 *   @node: inserted node.
 * Return:
 *   void
 ************************************************************************************/
void list_add_tail(struct list_node *head, struct list_node *node)
{
	struct list_node *tail = head->prev;

	node->prev = tail;
	node->next = head;
	tail->next = node;
	head->prev = node;
}

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
void list_add_front(struct list_node *head, struct list_node *node)
{
	struct list_node *front = head->next;

	node->prev = head;
	node->next = front;
	front->prev = node;
	head->next = node;
}

/*************************************************************************************
 * Function Name: list_del_tail
 * Description: Delete a tail node from the list.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   void
 ************************************************************************************/
void list_del_tail(struct list_node *head)
{
	struct list_node *tail_prev = head->prev->prev;

	tail_prev->next = head;
	head->prev = tail_prev;
}

/*************************************************************************************
 * Function Name: list_del_front
 * Description:  Delete a front node from the list.
 *
 * Param:
 *   @head: head node of the list.
 * Return:
 *   void
 ************************************************************************************/
void list_del_front(struct list_node *head)
{
	struct list_node *front_next = head->next->next;

	front_next->prev = head;
	head->next = front_next;
}