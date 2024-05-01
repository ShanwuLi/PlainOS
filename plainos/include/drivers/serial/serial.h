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

#ifndef __DRV_SERIAL_H__
#define __DRV_SERIAL_H__

#include <types.h>
#include <kernel/list.h>

struct serial_desc;
typedef void (*serial_recv_cb_t)(struct serial_desc *dev);

/*************************************************************************************
 * struct serial_ops:
 * Description:
 *   @send_char: synchronous sending char.
 *   @recv_char: synchronous reception char.
 *   @send_str: synchronous sending string.
 *   @register_recv_cb: register a callback function and call it when the received
 *                      character meet condition.
 *   @unregister_recv_cb: unregister a callback function.
 *
 * NOTE:
 *   Trigger_condition has a higher priority than length triggering. 
 ************************************************************************************/
struct serial_ops {
	int (*send_char)(struct serial_desc *desc, char c);
	int (*recv_char)(struct serial_desc *desc, char *c);
	int (*send_str)(struct serial_desc *desc, char *str);
	int (*register_recv_cb)(struct serial_desc *desc, int (*trigger_condition)(char c),
	                        size_t trigger_len, serial_recv_cb_t callback, void *arg);
	int (*unregister_recv_cb)(struct serial_desc *desc);
};

/*************************************************************************************
 * struct serial_ops:
 * Description:
 *   @id: the ID number of the serial port.
 *   @data_bits: data bits of the serial.
 *   @parity_bit: parity bit.
 *   @stop_bits: stop bit.
 *   @baud_rate: baud rate.
 *   @recv_buff_size: size of recv buffer.
 *   @recv_buff: recv buffer.
 *   @recv_callback: callback function for receiving character.
 *   @ops: operation of serial.
 *   @node: list node of serial_desc all registered.
 ************************************************************************************/
struct serial_desc {
	u8_t id;
	u8_t data_bits;
	u8_t parity_bit;
	u8_t stop_bits;
	u32_t baud_rate;
	size_t recv_buff_size;
	size_t trigger_length;
	void *recv_buff;
	int (*trigger_condition)(char c);
	serial_recv_cb_t recv_callback;
	struct serial_ops *ops;
	struct list_node node;
};


#elif /* __DRV_SERIAL_H__ */