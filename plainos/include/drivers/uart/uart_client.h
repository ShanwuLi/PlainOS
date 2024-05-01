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

#ifndef __DRIVER_UART_H__
#define __DRIVER_UART_H__

#include <types.h>




struct uart_client {
	uchar_t id;
	uchar_t parity_mode;
	uchar_t data_bits;
	uchar_t stop_bits;
	uint_t baud_rate;
	void *private;
};

typedef int (*uart_recv_callback_t)(struct uart_client *client, void *arg);

struct uart_recv_info {
	void *arg;
	size_t recv_size; /* if recv_size set to zero, it will determine by frame_end_data */
	char *recv_data;
	uint_t frame_end_data;
	uart_recv_callback_t callback;
};

int uart_transfer(struct uart_client *client, char *data_buff, size_t data_len);
int uart_recv_register(struct uart_client *client, struct uart_recv_info *info);


#endif /* __DRIVER_UART_H__ */
