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

#ifndef __KERNEL_KFIFO_H__
#define __KERNEL_KFIFO_H__

#include <types.h>
#include <errno.h>

struct pl_kfifo {
	volatile uint_t in;
	volatile uint_t out;
	uint_t size;
	char *buff;
};

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * 函数名称: pl_kfifo_init
 * 功能描述:
 *   初始化一个环形缓冲区（Kfifo）。为指定的环形缓冲区结构分配内存，并设置其内部状态.
 *
 * 参数:
 *   @kfifo: 环形缓冲区的句柄，通常是一个`struct kfifo`类型的指针.
 *   @buff: 环形缓冲区的数据缓冲区，用于存放数据.
 *   @buff_size: 数据缓冲区的大小，单位通常是字节.
 *
 * 返回值:
 *   如果初始化成功，返回初始化后的环形缓冲区句柄（即输入的`kfifo`参数）；如果初始化失败，可能返回NULL.
 ************************************************************************************/
int pl_kfifo_init(struct pl_kfifo *kfifo, char *buff, uint_t buff_size);

/*************************************************************************************
 * 函数名称: pl_kfifo_request
 * 功能描述:
 *   请求并初始化一个环形缓冲区（Kfifo）。此函数不仅初始化环形缓冲区，还会为其分配所需的内存空间.
 *
 * 参数:
 *   @buff_size: 数据缓冲区的大小，决定环形缓冲区能容纳数据的最大容量，单位通常是字节.
 *
 * 返回值:
 *   成功初始化并分配内存后的环形缓冲区句柄。这个句柄允许后续对缓冲区进行读写操作；如果请求失败，则可能返回NULL.
 ************************************************************************************/
struct pl_kfifo *pl_kfifo_request(uint_t buff_size);

/*************************************************************************************
 * 函数名称: pl_kfifo_destroy
 * 功能描述:
 *   销毁一个环形缓冲区（Kfifo）。释放与该缓冲区相关联的资源，包括内存缓冲区和结构体.
 *
 * 参数:
 *   @fifo: 环形缓冲区的句柄，通常是一个`struct kfifo`类型的指针.
 *
 * 返回值:
 *   无返回值，销毁操作完成后，不再可以使用该环形缓冲区句柄.
 ************************************************************************************/
void pl_kfifo_destroy(struct pl_kfifo *fifo);

/*************************************************************************************
 * 函数名称: pl_kfifo_len
 * 功能描述:
 *   获取环形缓冲区（Kfifo）中当前可用数据的长度，即已存入但未读取的数据量.
 *
 * 参数:
 *   @fifo: 环形缓冲区的句柄，通常是一个`struct kfifo`类型的指针.
 *
 * 返回值:
 *   返回环形缓冲区中当前可读数据的字节数.
 ************************************************************************************/
uint_t pl_kfifo_len(struct pl_kfifo *fifo);

/*************************************************************************************
 * 函数名称: pl_kfifo_put
 * 功能描述:
 *   将数据推入环形缓冲区（Kfifo）中。此函数尝试将给定的数据添加到缓冲区，返回实际添加到缓冲区的数据长度.
 *
 * 参数:
 *   @fifo: 环形缓冲区的句柄，通常是一个`struct kfifo`类型的指针.
 *   @data: 要写入缓冲区的数据.
 *   @data_len: 数据的长度，以字节为单位.
 *
 * 返回值:
 *   实际写入环形缓冲区的数据长度，如果写入了全部数据，则等于`data_len`，否则可能小于`data_len`.
 ************************************************************************************/
uint_t pl_kfifo_put(struct pl_kfifo *fifo, char *data, uint_t data_len);

/*************************************************************************************
 * 函数名称: pl_kfifo_get
 * 功能描述:
 *   从环形缓冲区（Kfifo）中获取数据。此函数尝试从缓冲区中读取数据，返回实际读取到的数据长度.
 *
 * 参数:
 *   @fifo: 环形缓冲区的句柄，通常是一个`struct kfifo`类型的指针.
 *   @data: 用于存储读取数据的缓冲区.
 *   @data_len: 预期要读取的数据长度，以字节为单位.
 *
 * 返回值:
 *   实际从环形缓冲区读取到的数据长度，如果读取了全部数据，则等于`data_len`，否则可能小于`data_len`.
 ************************************************************************************/
uint_t pl_kfifo_get(struct pl_kfifo *fifo, char *data, uint_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_KFIFO_H__ */
