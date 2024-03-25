#include <types.h>
#include <early_setup/early_uart.h>

//#define Task01StackSize 40
//#define Task02StackSize 40
//static uint32_t Task01Stack[Task01StackSize];
//static uint32_t Task02Stack[Task02StackSize];
//
//typedef struct TaskControlBlock{
//	uint32_t TaskStackPointer;
//}TCB;
//
//static TCB Task1TCB;
//static TCB Task1TCB;
//
//uint32_t Task01OnceEntry = 0;
//uint32_t Task02OnceEntry = 0;
//
//void TaskCreate(	void (*task_pointer)(void), 
//					uint32_t *TaskStackBasePointer, 
//					TCB TCBPointer);
//void TaskCreate(	void (*task_pointer)(void), 
//					uint32_t *TaskStackBasePointer, 
//					TCB TCBPointer)
//{
//
//}
//
//void Task01Scheduler(void){
//
//};
//
//void Task02Scheduler(void){
//	
//};
//
//void task01(void);
//void task02(void);
void delay(u16_t n);
//
void pl_callee_entry(void);
void pl_callee_entry(void)
{
	uint_t USART1_BaudRate = 115200;
	USART1_Init(USART1_BaudRate);
	while(1){
		USART1_PrintChar('1');
		delay(1000);
	};
};
//
//void task01(void){
//	while(1){
//		USART1_PrintChar('1');
//		delay(1000);
//	};
//};
//
//void task02(void){
//	while(1){
//		USART1_PrintChar('2');
//		delay(1000);
//	};
//};
//
void delay(u16_t n){
	int i = 0;
	for(i=0; i < n; ){
		i = i + 1;
	}
};