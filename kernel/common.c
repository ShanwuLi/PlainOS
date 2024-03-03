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

#include <types.h>
#include <port.h>
#include <kernel/kernel.h>

static volatile int pl_critical_ref = 0;

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
	pl_port_cpu_isb();
	++pl_critical_ref;
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
	if (pl_critical_ref == 0) {
		pl_port_unmask_interrupts();
		pl_port_cpu_isb();
	}
}
