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

#define pl_used                          __attribute__((used))
#define pl_const                         __attribute__((section("pl_const")))
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
	((struct_type *)((char *)(ptr) - (char *)(&(((struct_type *)0)->member))))

typedef int (*main_t)(int argc, char *argv[]);

#define pl_is_power_of_2(x)                    ((x) != 0 && (((x) & ((x) - 1)) == 0))
#define min(a, b)                              (((a) < (b)) ? (a) : (b))
#define max(a, b)                              (((a) > (b)) ? (a) : (b))

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_KERNEL_H__ */
