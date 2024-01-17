#include <pl_port.h>
#include "early_setup/early_uart.h"

int pl_early_port_putc_init(void)
{
	USART1_Init();

	return 0;
}

int pl_early_port_putc(char c)
{
	int ret = USART1_PrintChar(c);
	return ret;
}



pl_irqstate_t pl_port_irq_save(void)
{
	pl_irqstate_t irqstate;

	irqstate.state = 0;
	return irqstate;
}

void pl_port_irq_store(pl_irqstate_t irqstate)
{
	(void)irqstate;
}

void pl_port_schedule(void)
{

}

void *pl_port_task_stack_init(task_t task,
                                   void *task_stack,
                                   size_t stack_size)
{
	u32_t *stack = task_stack;
    stack      +=  stack_size;
    *(--stack)  = (u32_t)(1<<24); //XPSR
    *(--stack)  = (u32_t)task;    //PC
    *(--stack)  = 0ul;   //R0
    *(--stack)  = 0ul;   //R1
    *(--stack)  = 0ul;   //R2
    *(--stack)  = 0ul;   //R3
    *(--stack)  = 0ul;   //R12
    *(--stack)  = 0ul;   //LR

    *(--stack)  = 0ul;   //R4
    *(--stack)  = 0ul;   //R5
    *(--stack)  = 0ul;   //R6
    *(--stack)  = 0ul;   //R7
    *(--stack)  = 0ul;   //R8
    *(--stack)  = 0ul;   //R9
    *(--stack)  = 0ul;   //R10
    *(--stack)  = 0ul;   //R11
    return stack;
}

