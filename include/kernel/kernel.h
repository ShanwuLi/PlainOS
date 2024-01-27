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

#include "types.h"
#include <pl_cfg.h>

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

struct count {
	u32_t hi32;
	u32_t lo32;
};

#endif /* __KERNEL_KERNEL_H__ */
