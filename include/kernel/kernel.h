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

#ifndef __KERNEL_KERNEL_H__
#define __KERNEL_KERNEL_H__

#include <config.h>
#include <types.h>

#define __used                          __attribute__((used))
#define __const                         __attribute__((section("__const")))
#define USED(sym)                       ((void)(sym))
#define ARRAY_SIZE(a)                   (sizeof(a) / sizeof(a[0]))

/*************************************************************************************
 * Function Name: container_of
 * Description: Get the address of the structure instance.
 *
 * Param:
 *   @ptr: address of the structure member.
 *   @struct_type: type of the structure.
 *   @member: member name of the ptr in structure.
 * Return:
 *   void
 ************************************************************************************/
#define container_of(ptr, struct_type, member) \
	((struct_type *)((char *)ptr - (char *)(&(((struct_type *)0)->member))))

typedef int (*main_t)(int argc, char *argv[]);

struct count {
	u32_t hi32;
	u32_t lo32;
};

/*************************************************************************************
 * Function Name: pl_align_address
 * Description:
 *    Calculate the aligned address based on memory address and alignment coefficient.
 *
 * Param:
 *   @addr: the address of memory.
 *   @align: alignment coefficient.
 *
 * Return:
 *   Aligned address.
 ************************************************************************************/
void* pl_align_address(void* addr, uchar_t align);

/*************************************************************************************
 * Function Name: pl_align_size
 * Description:
 *    Calculate the aligned size.
 *
 * Param:
 *   @size: the size of need to align.
 *   @align: alignment coefficient.
 *
 * Return:
 *   Aligned size.
 ************************************************************************************/
size_t pl_align_size(size_t size, uchar_t align);

/*************************************************************************************
 * Function Name: pl_count_cmp
 * Description: compare count1 with count2.
 *
 * Parameters:
 *   @c1: count1.
 *   @c2: count2.
 *
 * Return:
 *   if c1  > c2: return > 0;
 *   if c1  < c2: return < 0;
 *   if c1 == c2: return 0.
 ************************************************************************************/
s32_t pl_count_cmp(struct count *c1, struct count *c2);

/*************************************************************************************
 * Function Name: pl_count_add
 * Description: add count1 and count2 to return.
 *
 * Parameters:
 *   @c: result = c1 + c2.
 *   @c1: count1.
 *   @c2: count2.
 *
 * Return:
 *   count added.
 ************************************************************************************/
int pl_count_add(struct count *c, struct count *c1, struct count *c2);

/*************************************************************************************
 * Function Name: void pl_enter_critical(void)
 * Description: enter critical area.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_enter_critical(void);

/*************************************************************************************
 * Function Name: void pl_exit_critical(void)
 * Description: exit critical area.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_exit_critical(void);


#endif /* __KERNEL_KERNEL_H__ */
