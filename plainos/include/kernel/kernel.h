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
 * 函数名称: container_of
 * 功能描述:
 *   通过结构体成员地址获取结构体实例的地址。这是一个宏，常用于链表操作或其他数据结构的遍历，帮助从已知成员追踪到其所属的结构体.
 *
 * 参数:
 *   @ptr: 结构体中某个成员的地址.
 *   @struct_type: 结构体的类型.
 *   @member: 在结构体中成员的名字，传入的是成员的名称而非成员的值或地址.
 *
 * 返回值:
 *   返回结构体实例的地址，类型为`struct_type *`.
 *
 * 注意:
 *   此处的"void"应修正为正确的返回类型说明，实际上`container_of`宏的返回类型应该是基于传入的`struct_type`.
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
 * 函数名称: pl_align_address
 * 功能描述:
 *   计算内存地址对齐后的地址。根据给定的内存地址和对齐系数，确保返回的地址满足指定的对齐要求.
 *
 * 参数:
 *   @addr: 待对齐的内存地址.
 *   @align: 对齐系数，通常为2的幂，如4（字节对齐）、8（双字对齐）等.
 *
 * 返回值:
 *   对齐后的内存地址，满足给定的对齐系数.
 ************************************************************************************/
void* pl_align_address(void* addr, uchar_t align);

/*************************************************************************************
 * 函数名称: pl_align_size
 * 功能描述:
 *   计算大小对齐后的结果。确保所提供的数据大小能够满足特定的对齐约束，通常用于分配内存时保证分配的内存区域按需对齐.
 *
 * 参数:
 *   @size: 需要对齐的原始大小.
 *   @align: 对齐系数，通常为2的幂次方数，例如4、8、16等，表示字节对齐的粒度.
 *
 * 返回值:
 *   对齐后的大小，确保是align的倍数且不小于原大小.
 ************************************************************************************/
size_t pl_align_size(size_t size, uchar_t align);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_KERNEL_H__ */
