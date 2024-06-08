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
#include <common/oschlogo.h>

#ifdef PL_CFG_OS_CHAR_LOGO
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
 * Function Name: pl_dis_os_char_logo，在终端上显示操作系统字符标识的Logo。
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
#ifdef PL_CFG_OS_CHAR_LOGO
    /* 初始化索引i用于遍历操作系统字符标识数组 */
    int i = 0;
    /* 从数组中读取第一个字符，并检查是否为结束标识'E' */
    char c = pl_port_rodata_read8(&pl_os_char_logo[i]);

    /* 循环直到遇到结束标识'E'，将每个字符输出到终端 */
    for (; c != 'E'; i++) {
        pl_port_putc(c);
        /* 读取下一个字符，继续循环 */
        c = pl_port_rodata_read8(&pl_os_char_logo[i]);
    }
#endif
}

/**
 * 对地址进行对齐操作
 * 
 * 本函数用于将给定的地址对齐到指定的字节边界。对齐对于某些硬件访问和数据结构优化是非常重要的。
 * 
 * @param addr 需要进行对齐的原始地址
 * @param align 对齐的字节大小。注意，align必须是2的幂次方，否则对齐操作的结果是未定义的。
 * 
 * @return 对齐后的地址。返回值是一个void指针，可以被转换为任何类型的指针使用。
 */
void* pl_align_address(void* addr, uchar_t align)
{
    /* 计算对齐掩码，align_mask的值等于align减1。这是因为对齐操作实际上是对地址加上对齐大小减1，然后取结果的低位。 */
    size_t align_mask = (size_t)align - 1;
    
    /* 将原始地址加上对齐掩码，然后通过位与操作取消掉低位中对齐大小减1的部分，从而实现地址对齐。 */
    /* 这里的位与操作使用了对齐大小的补码，即~align_mask，目的是保留高位，清零低位。 */
    return (void*)(((uintptr_t)addr + align_mask) & (~align_mask));
}

/**
 * 计算对齐后的大小
 * 
 * 本函数用于根据指定的对齐要求，计算出给定大小经过对齐后的结果。对齐对于内存分配和访问效率至关重要。
 * 
 * @param size 需要进行对齐的原始大小
 * @param align 对齐的基数，必须是2的幂次方
 * @return 返回对齐后的大小
 */
size_t pl_align_size(size_t size, uchar_t align)
{
    /* 计算对齐掩码，用于后续对齐操作 */
    size_t align_mask = (size_t)align - 1;
    
    /* 将原始大小与对齐掩码进行运算，以实现对齐效果，并通过位与操作移除多余的对齐位 */
    return (size + align_mask) & (~align_mask);
}
