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
#include <kernel/kfifo.h>
#include <kernel/workqueue.h>
#include <kernel/semaphore.h>

struct serial_desc;

/*************************************************************************************
 * struct serial_ops:
 * Description:
 *   @set_baud_rate: set buard rate for serial.
 *   @set_data_bits: set data bits for serial.
 *   @set_parity_bit: set parity bit for serial.
 *   @set_stop_bits: set stop bits for serial.
 *   @send_char: synchronous sending char.
 *   @recv_char: synchronous reception char.
 *   @send_str: synchronous sending string.
 *
 * NOTE:
 *   Trigger_condition has a higher priority than length triggering. 
 ************************************************************************************/
struct serial_ops {
	/* serial settings */
	int (*set_baud_rate)(struct serial_desc *desc, u32_t baud_rate);
	int (*set_data_bits)(struct serial_desc *desc, u8_t data_bits);
	int (*set_parity_bit)(struct serial_desc *desc, u8_t parity_bit);
	int (*set_stop_bits)(struct serial_desc *desc, u8_t stop_bits);
	/* serial functions */
	int (*send_char)(struct serial_desc *desc, char c);
	int (*recv_char)(struct serial_desc *desc, char *c);
	int (*send_str)(struct serial_desc *desc, char *str);
};

/*************************************************************************************
 * struct serial_recv_info:
 * Description:
 *   @buff_size: size of recv buffer.
 *   @buff_in: index of chars input.
 *   @buff_out: index of chars output.
 *   @ring_buff: ring buffer for receiving characters.
 *   @call_condition: call callback condition.
 *   @total_len: the length of total recevied characters.
 *   @chars: recevied characters at the moment.
 *   @chars_len: the length of recevied characters at the moment.
 *   @callback: callback function called when conditions are met.
 *   @cb_work: work for callback(will be executed in thread context).
 ************************************************************************************/
struct serial_recv_info {
	uint_t buff_size;
	uint_t buff_in;
	uint_t buff_out;
	char *ring_buff;
	int (*call_condition)(uint_t total_len, char *chars, uint_t chars_len);
	void (*callback)(struct serial_desc *desc);
	pl_work_t cb_work;
};

/*************************************************************************************
 * struct serial_ops:
 * Description:
 *   @port: the number of the serial port.
 *   @data_bits: data bits of the serial.
 *   @parity_bit: parity bit.
 *   @stop_bits: stop bit.
 *   @baud_rate: baud rate.
 *   @ops: serial's operations.
 *   @node: list node of serial_desc all registered.
 ************************************************************************************/
struct serial_desc {
	u8_t port;
	u8_t data_bits;
	u8_t parity_bit;
	u8_t stop_bits;
	u32_t baud_rate;
	struct serial_ops *ops;
	struct list_node node;
	struct pl_sem sem;
	struct serial_recv_info recv_info;
};

/*===================================== BSP Driver =================================*/
/*************************************************************************************
 * Function Name: pl_serial_desc_init
 * Description: initialize serial description
 *
 * Param:
 *   @desc: serial description.
 *   @port: serial port.
 *   @ops: operations of serial.
 *   @recv_buff_size: size of buffer for receiving chars, MUST BE POWER OF 2.
 *   @recv_buff: buffer for receiving chars, if is NULL,
 *               system will alloc memory for it.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_desc_init(struct serial_desc *desc, u8_t port, struct serial_ops *ops,
                                             uint_t recv_buff_size, void *recv_buff);

/*************************************************************************************
 * Function Name: pl_serial_desc_register
 * Description: register serial description
 *
 * Param:
 *   @desc: serial description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_desc_register(struct serial_desc *desc);

/*************************************************************************************
 * Function Name: pl_serial_callee_recv_handler
 * Description: the handler of serial when received characters.
 *
 * Param:
 *   @desc: serial description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_callee_recv_handler(struct serial_desc *desc, char *chars, uint_t chars_len);


/*===================================== Client Driver =================================*/
/*************************************************************************************
 * Function Name: pl_serial_set_baud_rate
 * Description: set baud rate for serial.
 *
 * Param:
 *   @desc: serial description.
 *   @baud_rate: baud rate of the serial.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_set_baud_rate(struct serial_desc *desc, u32_t baud_rate);

/*************************************************************************************
 * Function Name: pl_serial_set_data_bits
 * Description: set data bits for serial.
 *
 * Param:
 *   @desc: serial description.
 *   @data_bits: data bits of the serial.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_set_data_bits(struct serial_desc *desc, u8_t data_bits);

/*************************************************************************************
 * Function Name: pl_serial_set_parity_bit
 * Description: set parity bit for serial.
 *
 * Param:
 *   @desc: serial description.
 *   @parity_bit: parity bit of the serial.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_set_parity_bit(struct serial_desc *desc, u8_t parity_bit);

/*************************************************************************************
 * Function Name: pl_serial_set_stop_bits
 * Description: set stop bit for serial.
 *
 * Param:
 *   @desc: serial description.
 *   @stop_bits: stop bits of the serial.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_set_stop_bits(struct serial_desc *desc, u8_t stop_bits);

/*************************************************************************************
 * Function Name: pl_serial_send_char
 * Description: send a character by the serial.
 *
 * Param:
 *   @desc: serial description.
 *   @c: character.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_send_char(struct serial_desc *desc, char c);

/*************************************************************************************
 * Function Name: pl_serial_send_str
 * Description: send a string by the serial.
 *
 * Param:
 *   @desc: serial description.
 *   @str: the string you want to send.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_send_str(struct serial_desc *desc, char *str);


#endif /* __DRV_SERIAL_H__ */
