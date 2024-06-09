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
#include <port/port.h>
#include <kernel/kernel.h>
#include <common/oschlogo.h>

#ifdef CONFIG_PL_CFG_OS_CHAR_LOGO
static char pl_const pl_os_char_logo[] = \
" ______   __            __             ______    ______  \r\n\
/       \\ /  |          /  |           /      \\  /      \\ \r\n\
$$$$$$$  |$$ |  ______  $$/  _______  /$$$$$$  |/$$$$$$  |\r\n\
$$ |__$$ |$$ | /      \\ /  |/       \\ $$ |  $$ |$$ \\__$$/ \r\n\
$$    $$/ $$ | $$$$$$  |$$ |$$$$$$$  |$$ |  $$ |$$      \\ \r\n\
$$$$$$$/  $$ | /    $$ |$$ |$$ |  $$ |$$ |  $$ | $$$$$$  |\r\n\
$$ |      $$ |/$$$$$$$ |$$ |$$ |  $$ |$$ \\__$$ |/  \\__$$ |\r\n\
$$ |      $$ |$$    $$ |$$ |$$ |  $$ |$$    $$/ $$    $$/ \r\n\
$$/       $$/  $$$$$$$/ $$/ $$/   $$/  $$$$$$/   $$$$$$/  \r\n\
                                                       \r\nE";
#endif

/*************************************************************************************
 * Function Name: pl_dis_os_char_logo
 * Description:
 *    Display character logo.
 *
 * Param:
 *   none.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_dis_os_char_logo(void)
{
#ifdef CONFIG_PL_CFG_OS_CHAR_LOGO
	int i = 0;
	char c = pl_port_rodata_read8(&pl_os_char_logo[i]);

	for (; c != 'E'; i++) {
		pl_port_putc(c);
		c = pl_port_rodata_read8(&pl_os_char_logo[i]);
	}
#endif
}

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
