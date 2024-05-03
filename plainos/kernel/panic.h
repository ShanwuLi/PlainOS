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

#ifndef __KERNEL_PANIC_PRIVATE_H__
#define __KERNEL_PANIC_PRIVATE_H__

#include "task.h"

enum panic_reason {
	PL_PANIC_REASON_STACKOVF = 0,
	PL_PANIC_REASON_DIVZERO,
	PL_PANIC_REASON_DEADLOCK,
};

/*************************************************************************************
 * Function Name: pl_panic_dump
 * Description: dump task info when system panic.
 *
 * Param:
 *   @panic_tcb: tcb of panic task.
 *   @panic_reason: reason for panic.
 *   @arg: argument.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_panic_dump(struct tcb *panic_tcb, u32_t panic_reason, void *arg);

#endif /* __KERNEL_PANIC_PRIVATE_H__ */
