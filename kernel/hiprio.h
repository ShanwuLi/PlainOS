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
#ifndef __KERNEL_HIPRIO_H__
#define __KERNEL_HIPRIO_H__

#include <kernel/kernel.h>

/*************************************************************************************
 * Function Name: pos_get_hiprio
 * Description: Get current highest priority.
 *
 * Param:
 *   void
 *
 * Return:
 *   @hiprio: current highest priority
 ************************************************************************************/
u16_t pos_get_hiprio(void);

/*************************************************************************************
 * Function Name: pos_clear_bit_of_hiprio_bitmap
 * Description: clear the bit of highest priority table.
 *
 * Param:
 *   @prio: priority
 *
 * Return:
 *   void
 ************************************************************************************/
void pos_clear_bit_of_hiprio_bitmap(u16_t prio);

/*************************************************************************************
 * Function Name: pos_set_bit_of_hiprio_bitmap
 * Description: set the bit of highest priority table.
 *
 * Param:
 *   @prio: priority
 *
 * Return:
 *   void
 ************************************************************************************/
void pos_set_bit_of_hiprio_bitmap(u8_t prio);

#endif /* __KERNEL_HIPRIO_H__ */