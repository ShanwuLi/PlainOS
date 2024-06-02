#include <types.h>
#include <port.h>
#include <task.h>
#include <early_setup/early_uart.h>
#include <cmsis/cmsis_gcc.h>
#include <stm32f10x_it.h>

//宏定义任务栈大小
#define TaskNumbers 4
#define IdleTaskStackSize 256
#define Task01StackSize 256
#define Task02StackSize 256
#define Task03StackSize 256
//用数组存储空间构建任务栈空间存放Context内容，全局变量不显式调用static
static uint32_t IdleTaskStack[IdleTaskStackSize];
static uint32_t Task01Stack[Task01StackSize];
static uint32_t Task02Stack[Task02StackSize];
static uint32_t Task03Stack[Task03StackSize];
//为任务创建任务控制块数组
static TCB TaskControlBlockArray[TaskNumbers];
TCB *TaskControlBlockArrayPointer;
uint8_t Task_Numbers = TaskNumbers;

void delay(uint32_t n);
void delay(uint32_t n){
	u32_t volatile i = 0;
	for(i=0; i < n; ){
		i = i + 1;
	}
}

void task(void *arg);
void task(void *arg){
	while(1){
		USART1_PrintChar('T');
		USART1_PrintChar('A');
		USART1_PrintChar('S');
		USART1_PrintChar('K');
		USART1_PrintChar((char)((uint32_t)arg));
		USART1_PrintChar('\t');
		delay(1000000);
	};
}

void idle_task(void *arg);
void idle_task(void *arg){
	pl_port_systick_init();
	while(1){
		USART1_PrintChar('T');
		USART1_PrintChar('A');
		USART1_PrintChar('S');
		USART1_PrintChar('K');
		USART1_PrintChar((char)((uint32_t)arg));
		USART1_PrintChar('\t');
		delay(1000000);
	};
}

void pl_callee_entry(void);
void pl_callee_entry(){
	Task_Numbers = TaskNumbers;
	TaskControlBlockArrayPointer = TaskControlBlockArray;
	uint_t USART1_BaudRate = 115200;
	USART1_Init(USART1_BaudRate);
	USART1_PrintChar('S');
	USART1_PrintChar('\t');
	pl_task_sys_create(idle_task, IdleTaskStack, IdleTaskStackSize, &TaskControlBlockArray[0], (void *)'I');
	pl_task_sys_create(task, Task01Stack, Task01StackSize, &TaskControlBlockArray[1], (void *)'1');
	pl_task_sys_create(task, Task02Stack, Task02StackSize, &TaskControlBlockArray[2], (void *)'2');
	pl_task_sys_create(task, Task03Stack, Task03StackSize, &TaskControlBlockArray[3], (void *)'3');
	pl_port_switch_context();
	while(1);
}

