#include "early_uart.h"
#include <port.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile int pl_critical_ref = 0;

static usart USART1={
	.ux_cofg = {
		.number = 0,
		.osc_hz = OSC_16000000,
		.ubrr   = UBRR_115200,
		.data_bits_num = DATA_BITS_8,
		.stop_bits_num = STOP_BITS_2,
		.crc_type = CRC_FORBID,
		.wait_or_interrupt = 0
	},
};

int main(void)
{
	pl_callee_entry();
}

int pl_port_putc_init(void)
{
	usart_init(&USART1);
	USART1.send_str(&USART1, (char *)"pl_port_putc_init\r\n");
	return 0;
}

int pl_port_putc(const char c)
{
	USART1.transmit_data(&USART1, c);
	return 0;
}

int pl_port_systick_init(void)
{
    TCCR1B = 0x02;   //进行2预分频  16MHz
    TCNT1H = 0XFF;
    TCNT1L = 0X00;
    OCR1AH = 0XFF;
    OCR1AL = 0XFF;
    TIFR1  = 0X00;
    SREG &= (u8_t)(~(1<<7));     /*< 关中断 */
	TIMSK1 |= (u8_t)(1<<0);

	pl_port_putc('S');
	return 0;
}

ISR(TIMER1_OVF_vect)
{
	pl_callee_systick_expiration();
}

/*************************************************************************************
 * Function Name: void pl_port_enter_critical(void)
 * Description: enter critical area.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_enter_critical(void)
{
	SREG &= (u8_t)(~(1<<7));     /*< 关中断 */
	++pl_critical_ref;
	pl_port_cpu_isb();
}

/*************************************************************************************
 * Function Name: void pl_port_exit_critical(void)
 * Description: exit critical area.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_exit_critical(void)
{
	--pl_critical_ref;
	if (pl_critical_ref == 0)
		SREG |= (u8_t)(1<<7);     /*< 开中断 */
	pl_port_cpu_isb();
}

void *pl_port_task_stack_init(void *task, void *task_stack, size_t stack_size,
                              void **context_top_sp, void *param)
{
	char *stack = task_stack;

    *context_top_sp = stack;
    stack       +=  stack_size - 1;

    *(stack--)  =   (u8_t)((u16_t)task);
    *(stack--)  =   (u8_t)((u16_t)task>>8);
    *(stack--)  =   (u8_t)(0);
    *(stack--)  =   0u;   //r1
    *(stack--)  =   0u;   //r0
    *(stack--)  =   (u8_t)(1 << 7);   /* SREG */
    *(stack--)  =   0u;   //RAMP
    *(stack--)  =   0u;   //r2
    *(stack--)  =   0u;   //r3
    *(stack--)  =   0u;   //r4
    *(stack--)  =   0u;   //r5
    *(stack--)  =   0u;   //r6
    *(stack--)  =   0u;   //r7
    *(stack--)  =   0u;   //r8
    *(stack--)  =   0u;   //r9
    *(stack--)  =   0u;   //r10
    *(stack--)  =   0u;   //r11
    *(stack--)  =   0u;   //r12
    *(stack--)  =   0u;   //r13
    *(stack--)  =   0u;   //r14
    *(stack--)  =   0u;   //r15
    *(stack--)  =   0u;   //r16
    *(stack--)  =   0u;   //r17
    *(stack--)  =   0u;   //r18
    *(stack--)  =   0u;   //r19
    *(stack--)  =   0u;   //r20
    *(stack--)  =   0u;   //r21
    *(stack--)  =   0u;   //r22
    *(stack--)  =   0u;   //r23
    *(stack--)  =   (u8_t)(u16_t)param;   //r24
    *(stack--)  =   (u8_t)((u16_t)param >> 8);   //r25
    *(stack--)  =   0u;   //r26
    *(stack--)  =   0u;   //r27
    *(stack--)  =   0u;   //r30
    *(stack--)  =   0u;   //r31
    *(stack--)  =   0u;   //r29
    *(stack--)  =   0u;   //r28

    return stack;
}