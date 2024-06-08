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
		.next = &(name), \
		.prev = &(name)  \
	}

#define LIST_INIT_VAL(name)  { \
		.prev = &(name), \
		.next = &(name)  \
	}

/*************************************************************************************
 * 函数名称: list_for_each
 * 功能描述:
 *   遍历链表的每一个节点。此宏从指定的链表头开始，依次访问链表中的每个节点，直到到达链表的尾部。不推荐在遍历过程中删除或修改节点，因为这可能导致迭代器失效.
 *
 * 参数:
 *   @pos: 类型为`struct list_head *`的指针，用作循环中的迭代器或游标，初始时指向`list_head`.
 *   @list_head: 链表头部的`struct list_head`指针，标识链表的起始位置.
 *
 * 返回值:
 *   无返回值。此宏用于控制循环流程，允许在循环体内访问链表中的每个节点.
 ************************************************************************************/
#define list_for_each(pos, list_head) \
	for ((pos) = (list_head)->next; (pos) != (list_head); (pos) = (pos)->next)

/*************************************************************************************
 * 函数名称: list_next_entry
 * 功能描述:
 *   获取指定节点在链表中的下一个节点。此函数通过给定的结构体实例和链表节点成员名称，找到并返回链表中紧接在当前节点之后的节点实例.
 *
 * 参数:
 *   @entry: 当前处理的链表节点对应的结构体实例.
 *   @entry_type: 当前节点所属的结构体类型.
 *   @member: 在`entry_type`结构体内定义的`struct list_head`类型的成员名称，用于链接各个结构体实例.
 *
 * 返回值:
 *   @entry_type: 指定节点的下一个节点对应的结构体实例。如果没有下一个节点，则根据实现可能会有不同的处理方式（如返回NULL）.
 ************************************************************************************/
#define list_next_entry(entry, entry_type, member) \
	container_of((entry)->member.next, entry_type, member)

/*************************************************************************************
 * 函数名称: list_first_entry
 * 功能描述:
 *   返回特定链表中的第一个条目。此函数通过链表头指针、条目类型以及链表节点成员名称，获取链表头部指向的第一个结构体实例.
 *
 * 参数:
 *   @head: 链表的头节点，通常是一个`struct list_head`类型的变量，表示链表的起始位置.
 *   @entry_type: 链表中节点对应的结构体类型.
 *   @member: 在`entry_type`结构体内定义的`struct list_head`类型的成员名称，该成员用于构成链表.
 *
 * 返回值:
 *   @entry_type: 链表中的第一个条目，即链表头节点之后的第一个有效结构体实例。如果链表为空，根据实现可能会返回NULL或其他表示空值的方式.
 ************************************************************************************/
#define list_first_entry(head, entry_type, member) \
	container_of((head)->next, entry_type, member)

/*************************************************************************************
 * 函数名称: list_prev_entry
 * 功能描述:
 *   返回指定节点在链表中的前一个条目。此函数利用给定的结构体实例、结构体类型以及链表节点成员名称，获取当前节点之前的一个结构体实例.
 *
 * 参数:
 *   @entry: 需要查询其前驱节点的结构体实例.
 *   @entry_type: `entry`所对应的结构体类型.
 *   @member: 在`entry_type`结构体内声明的`struct list_head`类型的成员名称，该成员参与构成链表.
 *
 * 返回值:
 *   @entry_type: 如果存在，返回`entry`之前的前一个结构体实例；如果`entry`是链表的首个节点或链表为空，则返回NULL或其他表示无前驱的值.
 ************************************************************************************/
#define list_prev_entry(entry, entry_type, member) \
	container_of((entry)->member.prev, entry_type, member)

/*************************************************************************************
 * 函数名称: list_for_each_entry
 * 功能描述: 遍历结构体中嵌入的链表元素。
 *
 * 此宏用于遍历一个链表，其中链表的每个节点都是更大结构体的一部分，允许访问与链表节点关联的实际结构体数据。
 *
 * 参数:
 *   @pos: 使用&struct list_head作为循环游标的指针，指向结构体实例中的list_head成员。
 *   @list_head: 链表的头节点，表示遍历的起始位置。
 *   @entry_type: 包含list_head成员的结构体类型。
 *   @member: 在entry_type结构体中，list_head成员的名称。
 *
 * 返回:
 *   无返回值。此宏用于控制循环，允许在循环体内处理链表中的每个结构体实例。
 ************************************************************************************/
#define list_for_each_entry(pos, list_head, entry_type, member) \
	for ((pos) = container_of((list_head)->next, entry_type, member); \
	     &(pos)->member != (list_head); \
	     (pos) = container_of((pos)->member.next, entry_type, member))

/*************************************************************************************
 * 函数名称: list_for_each_entry_safe
 * 功能描述: 安全遍历结构体中嵌入的链表元素。
 *
 * 此宏在遍历链表时提供额外的安全保障，允许在遍历过程中修改或删除链表节点而不会引发错误。通过引入一个临时变量，确保迭代过程中的指针安全。
 *
 * 参数:
 *   @pos: 用作循环游标的指针，类型为结构体指针，指向当前正在访问的元素。
 *   @temp: 临时存储指针，同样为结构体指针，用于暂存下一个待访问元素，确保迭代安全。
 *   @list_head: 链表的头节点指针，标识遍历的起点。
 *   @entry_type: 结构体的类型，包含了链表节点作为其成员之一。
 *   @member: 在entry_type结构体内，链表节点成员的名称。
 *
 * 返回:
 *   无返回值。宏用于控制循环逻辑，确保遍历过程中的操作安全执行。
 ************************************************************************************/
#define list_for_each_entry_safe(pos, temp, list_head, entry_type, member) \
	for ((pos) = container_of((list_head)->next, entry_type, member), \
	     (temp) = list_next_entry(pos, entry_type, member); \
	     &(pos)->member != (list_head); \
	     (pos) = (temp), (temp) = list_next_entry((temp), entry_type, member))


#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * 函数名称: list_init
 * 功能描述: 初始化一个链表.
 *
 * 此函数负责设置链表节点的next和prev指针，使其自指，从而创建一个空链表或初始化已有链表的头部节点，准备后续的节点插入操作.
 *
 * 参数:
 *   @node: 需要初始化的链表节点指针，通常是链表头节点.
 *
 * 返回值:
 *   无返回值. 函数直接修改传入节点的指针域，不返回任何内容.
 ************************************************************************************/
void list_init(struct list_node *node);

/*************************************************************************************
 * 函数名称: list_get_node_num
 * 功能描述: 获取链表中的节点数量.
 *
 * 此函数遍历链表，从头节点开始计数，直到达到链表尾部，返回链表中总共有多少个节点.
 *
 * 参数:
 *   @head: 链表的头节点指针，用于开始计数.
 *
 * 返回值:
 *   int: 链表中节点的数量.
 ************************************************************************************/
int list_get_node_num(struct list_node *head);

/*************************************************************************************
 * 函数名称: list_add_node_at_tail
 * 功能描述: 将节点添加到链表的末尾.
 *
 * 此函数接收链表的头节点和要插入的新节点，将新节点正确地连接到链表的末尾，以保持链表的完整性.
 *
 * 参数:
 *   @head: 链表的头节点指针.
 *   @node: 要插入到链表中的新节点.
 *
 * 返回值:
 *   无返回值. 函数直接修改链表结构，添加新节点.
 ************************************************************************************/
void list_add_node_at_tail(struct list_node *head, struct list_node *node);

/*************************************************************************************
 * 函数名称: list_add_node_at_front
 * 功能描述: 将节点添加到链表的前端.
 *
 * 此函数负责在链表的开头插入一个新节点，即新节点将成为新的头节点，原头节点之后的所有节点依次后移一位.
 *
 * 参数:
 *   @head: 链表的当前头节点指针.
 *   @node: 待插入链表的节点指针.
 *
 * 返回值:
 *   无返回值. 直接修改链表，将新节点添加至链表前端.
 ************************************************************************************/
void list_add_node_at_front(struct list_node *head, struct list_node *node);

/*************************************************************************************
 * 函数名称: list_add_node_behind
 * 功能描述: 在链表的后面插入一个新节点.
 *
 * 参数:
 *   @head: 链表的头节点指针.
 *   @node: 要插入的节点指针.
 *
 * 返回值:
 *   无返回值. 直接修改链表，将新节点添加至指定位置.
 ************************************************************************************/
void list_add_node_behind(struct list_node *pos, struct list_node *node);

/*************************************************************************************
 * 函数名称: list_add_node_ahead
 * 功能描述: 在链表的前面插入一个新节点，即将新节点设为新的头节点.
 *
 * 参数:
 *   @head: 当前的链表头节点指针，新节点将被添加在其前面并取代其成为新的头节点.
 *   @node: 要插入的新节点指针.
 *
 * 返回值:
 *   无返回值. 函数直接修改链表结构，将新节点添加至链表前面.
 ************************************************************************************/
void list_add_node_ahead(struct list_node *pos, struct list_node *node);

/*************************************************************************************
 * 函数名称: list_del_tail_node
 * 功能描述: 从链表中删除尾部节点.
 *
 * 此函数从链表中移除最后一个节点（尾节点），并返回该节点指针. 在操作完成后，链表的倒数第二个节点将成为新的尾节点.
 *
 * 参数:
 *   @head: 链表的头节点指针，用于定位整个链表.
 *
 * 返回值:
 *   @struct list_node*: 被删除的尾部节点的指针.
 ************************************************************************************/
struct list_node *list_del_tail_node(struct list_node *head);

/*************************************************************************************
 * 函数名称: list_del_front_node
 * 功能描述: 从链表中删除前端节点（头节点）.
 *
 * 此函数负责移除链表中的第一个节点，并将链表的第二个节点提升为新的头节点. 函数返回被删除的原头节点的指针.
 *
 * 参数:
 *   @head: 链表的头节点指针，此指针将被更新以指向新的头节点.
 *
 * 返回值:
 *   @struct list_node*: 被删除的前端节点的指针.
 ************************************************************************************/
struct list_node *list_del_front_node(struct list_node *head);

/*************************************************************************************
 * 函数名称: list_del_node
 * 功能描述: 从链表中删除指定节点.
 *
 * 此函数接受一个链表中的节点指针，并将其从链表中移除，同时更新其前一个和后一个节点的链接，以保持链表的连续性.
 *
 * 参数:
 *   @node: 需要从链表中删除的节点指针.
 *
 * 返回值:
 *   无返回值. 函数直接修改链表结构以删除指定节点.
 ************************************************************************************/
void list_del_node(struct list_node *node);

/*************************************************************************************
 * 函数名称: list_is_empty
 * 功能描述: 判断链表是否为空.
 *
 * 此函数检查链表的头节点是否指向自身，以此判断链表是否为空.
 *
 * 参数:
 *   @head: 链表的头节点指针.
 *
 * 返回值:
 *   @bool: 如果链表为空，返回true；否则返回false.
 ************************************************************************************/
bool list_is_empty(struct list_node *head);

/*************************************************************************************
 * 函数名称: list_next_node
 * 功能描述: 获取指定节点的下一个节点.
 *
 * 此函数接收链表中一个节点的指针，返回该节点的下一个节点指针. 如果给定节点是链表的最后一个节点，返回NULL表示没有下一个节点.
 *
 * 参数:
 *   @node: 指定的链表节点.
 *
 * 返回值:
 *   @list_node*: 指定节点的下一个节点指针；如果无下一个节点，返回NULL.
 ************************************************************************************/
struct list_node* list_next_node(struct list_node *node);

/*************************************************************************************
 * 函数名称: list_prev_node
 * 功能描述: 获取指定节点的前一个节点.
 *
 * 此函数接收链表中一个节点的指针，返回该节点的前一个节点指针. 如果给定节点是链表的第一个节点，返回NULL表示没有前一个节点.
 *
 * 参数:
 *   @node: 指定的链表节点.
 *
 * 返回值:
 *   @list_node*: 指定节点的前一个节点指针；如果无前一个节点，返回NULL.
 ************************************************************************************/
struct list_node* list_prev_node(struct list_node *node);

/*************************************************************************************
 * 函数名称: list_move_chain_to_node_behind
 * 功能描述: 将一个子链表移动到指定节点之后.
 *
 * 此函数将一个以`src_front`为头节点，以`src_tail`为尾节点的子链表从当前位置移出，并插入到`dst_node`之后.
 * 移动后，`src_tail`的下一个节点将指向`dst_node`的下一个节点，而`dst_node`的下一个节点将指向`src_front`.
 *
 * 参数:
 *   @dst_node: 目标位置的节点，子链表将被放置在此节点之后.
 *   @src_front: 源子链表的头节点.
 *   @src_tail: 源子链表的尾节点.
 *
 * 返回值:
 *   无返回值. 函数直接修改链表结构完成子链表的移动操作.
 ************************************************************************************/
void list_move_chain_to_node_behind(struct list_node *dst_node,
      struct list_node *src_front, struct list_node *src_tail);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_LIST_H__ */
