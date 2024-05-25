#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#include <types.h>
#include "stm32f10x.h"

void USART1_IRQHandler(void);
void pl_sys_dump(u32_t *hardfault_args);

#endif /* __STM32F10x_IT_H */

