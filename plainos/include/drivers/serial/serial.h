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
#include <kernel/semaphore.h>

struct serial_desc;
typedef void (*serial_recv_cb_t)(struct serial_desc *desc);

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
	/* serial settings */
	int (*set_baud_rate)(struct serial_desc *desc, u32_t baud_rate);
	int (*set_data_bits)(struct serial_desc *desc, u8_t data_bits);
	int (*set_parity_bit)(struct serial_desc *desc, u8_t parity_bit);
	int (*set_stop_bits)(struct serial_desc *desc, u8_t stop_bits);
	/* serial functions */
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
 *   @port: the number of the serial port.
 *   @data_bits: data bits of the serial.
 *   @parity_bit: parity bit.
 *   @stop_bits: stop bit.
 *   @baud_rate: baud rate.
 *   @recv_buff_size: size of recv buffer.
 *   @trigger_length: trigger call callback when received chars reach the length.
 *   @recv_buff: recv buffer.
 *   @trigger_condition: trigger call callback when trigger_condition return truth.
 *   @recv_callback: callback function for receiving character.
 *   @ops: serial's operations.
 *   @node: list node of serial_desc all registered.
 ************************************************************************************/
struct serial_desc {
	u8_t port;
	u8_t data_bits;
	u8_t parity_bit;
	u8_t stop_bits;
	u32_t baud_rate;
	size_t recv_buff_size;
	size_t trigger_length;
	pl_kfifo_handle recv_buff;
	int (*trigger_condition)(struct serial_desc *desc, char *chars, size_t len);
	serial_recv_cb_t recv_callback;
	struct serial_ops *ops;
	struct list_node node;
	pl_semaphore_handle_t sem;
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
                                             size_t recv_buff_size, void *recv_buff);

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
 * Function Name: pl_serial_recv_handler
 * Description: the handler of serial when received characters.
 *
 * Param:
 *   @desc: serial description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_recv_handler(struct serial_desc *desc, char *chars, size_t len);


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
