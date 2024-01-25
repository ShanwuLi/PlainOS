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

#ifndef __PLAINOS_PORT_H__
#define __PLAINOS_PORT_H__

#include <stddef.h>
#include <kernel/kernel.h>
#include <kernel/task.h>
#include <types.h>

/*================================== rodata ports =======================================*/
u8_t pl_port_rodata_read8(void *addr);
u16_t pl_port_rodata_read16(void *addr);
u32_t pl_port_rodata_read32(void *addr);
uintptr_t pl_port_rodata_read(void *addr);

/*================================== early ports =======================================*/
void pl_entry(void);
int pl_early_port_putc_init(void);
int pl_early_port_putc(const char c);

/*=================================== task ports =======================================*/
typedef struct irqstate
{
	unsigned int state;
} irqstate_t;

irqstate_t pl_port_irq_save(void);
void pl_port_irq_store(irqstate_t);
void pl_port_schedule(void);
void *pl_port_task_stack_init(task_t task, void *task_stack, size_t stack_size);

#endif /* __PLAINOS_PORT_H__ */
