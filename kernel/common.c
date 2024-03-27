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

#include <errno.h>
#include <types.h>
#include <port.h>
#include <kernel/kernel.h>

static volatile int pl_critical_ref = 0;

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
void* pl_align_address(void* addr, uchar_t align)
{
	size_t align_mask = (size_t)align - 1;
	return (void*)(((uintptr_t)addr + align_mask) & (~align_mask));
}

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
size_t pl_align_size(size_t size, uchar_t align)
{
	size_t align_mask = (size_t)align - 1;
	return (size + align_mask) & (~align_mask);
}

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
s32_t pl_count_cmp(struct count *c1, struct count *c2)
{
	s32_t hi_diff = c1->hi32 - c2->hi32;
	s32_t lo_diff = c1->lo32 - c2->lo32;

	return hi_diff == 0 ? lo_diff : hi_diff;
}

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
int pl_count_add(struct count *c, struct count *c1, struct count *c2)
{
	u32_t hi = c1->hi32 + c2->hi32;
	u32_t lo = c1->lo32 + c2->lo32;

	if (lo < c1->lo32 || lo < c2->lo32)
		hi++;
	
	c->hi32 = hi;
	c->lo32 = lo;
	return OK;
}

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
void pl_enter_critical(void)
{
	pl_port_mask_interrupts();
	++pl_critical_ref;
	pl_port_cpu_isb();
}

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
void pl_exit_critical(void)
{
	--pl_critical_ref;
	if (pl_critical_ref == 0)
		pl_port_unmask_interrupts();
	pl_port_cpu_isb();
}
