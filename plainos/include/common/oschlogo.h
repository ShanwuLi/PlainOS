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

#ifndef __COMMON_OSCHLOGO_H__
#define __COMMON_OSCHLOGO_H__

#include <config.h>
#include <kernel/kernel.h>

/***************************************************************************************
 * 函数名称: pl_dis_os_char_logo
 * 功能描述:
 *    此函数负责显示字符形式的Logo。Logo通常由特定排列的字符组成，用于在终端或控制台
 *    上展示程序或操作系统的标识信息，增强用户界面的可识别性。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 *************************************************************************************/
void pl_dis_os_char_logo(void);

#endif /* __COMMON_OSCHLOGO_H__ */
