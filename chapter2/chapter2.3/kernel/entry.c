#include <types.h>
#include <early_setup/early_uart.h>
#include <cmsis/cmsis_gcc.h>
#include <stm32f10x_it.h>

//用数组存储空间构建任务栈空间存放Context内容，全局变量不显式调用static
#define TaskNumbers		4
#define IdleTaskStackSize 256
#define Task01StackSize 256
#define Task02StackSize 256
#define Task03StackSize 256

static uint32_t IdleTaskStack[IdleTaskStackSize];
static uint32_t Task01Stack[Task01StackSize];
static uint32_t Task02Stack[Task02StackSize];
static uint32_t Task03Stack[Task03StackSize];



typedef struct TaskControlBlock{
	uint32_t volatile TaskStackPointer;
}TCB;
static TCB TaskControlBlockArray[TaskNumbers];

//首次运行任务内容前需要将MCU状态送入各自的任务栈空间内，void (*Task)(void) Task变量指向函数，此处形式参数用于传递函数地址
void  TaskCreate(void (*Task)(void *arg), uint32_t *TaskStack, 
				uint32_t StackSize, TCB *TaskControlBlock, void *arg);
//创建任务控制块数组记录任务堆栈指针变化情况
void  TaskCreate(void (*Task)(void *arg), uint32_t *TaskStack, 
				uint32_t StackSize, TCB *TaskControlBlock, void *arg)
{
	uint32_t *stack = (uint32_t *)TaskStack;
	stack = stack + StackSize; //为了方便理解这里将任务栈存储空间设置为Nx32结构和STM32寄存器位数保持一样，确保SP入栈一次减少4,stack变量减少1

	*(--stack)  = (u32_t)(1<<24);  /* XPSR */ //这里入栈顺序和《Cortex-M3权威指南9.1中断/异常的响应序列》一样，注意软件入栈顺序和硬件底层流水线入栈有些不一样
	*(--stack)  = (u32_t)Task;     /* PC ,将任务地址压栈*/
	*(--stack)  = (u32_t)0;        /* LR */
	*(--stack)  = (u32_t)0;        /* R12 */
	*(--stack)  = (u32_t)0;        /* R3 */
	*(--stack)  = (u32_t)0;        /* R2 */
	*(--stack)  = (u32_t)0;        /* R1 */
	*(--stack)  = (u32_t)arg;      /* R0 */



	*(--stack)  = (u32_t)0;        /* R4 */	//这里入栈顺序依照用户喜欢的顺序，这里就按照寄存器名称顺序入栈
	*(--stack)  = (u32_t)0;        /* R5 */
	*(--stack)  = (u32_t)0;        /* R6 */
	*(--stack)  = (u32_t)0;        /* R7 */
	*(--stack)  = (u32_t)0;        /* R8 */
	*(--stack)  = (u32_t)0;        /* R9 */
	*(--stack)  = (u32_t)0;        /* R10 */
	*(--stack)  = (u32_t)0;        /* R11 */
	TaskControlBlock->TaskStackPointer = (uint32_t)stack; //使用全局变量TaskControlBlock->TaskStackPointer成员记录个任务栈SP位置
}


//USART1_PrintChar在中断中可能会扰乱LR寄存器数值从而影响程序PC返回值与MCU状态
int8_t volatile TaskID = -1;

#define PL_CFG_SYSTICK_TIME_SLICE_US 100000
void pl_port_systick_init(void);
//RTS OS滴答定时器初始化，移植时需要用户自己实现
#define PendSV_Priority_Level_Register 0xE000ED22
#define PendSV_Priority_Value 0xFF

void pl_port_systick_init(void)
{
	//__asm__ volatile(".equ	PendSV_Priority_Level_Register,	0xE000ED22\n\t");
	//__asm__ volatile(".equ	PendSV_Priority_Value,	0xFF\n\t");
	//__asm__ volatile("LDR	R0,	PendSV_Priority_Level_Register\n\t");
	//__asm__ volatile("LDR	R1,	PendSV_Priority_Value\n\t");
	//__asm__ volatile("STRB	R1,	[R0]\n\t");
	(*(uint32_t *)PendSV_Priority_Level_Register) = PendSV_Priority_Value;

	/** 填写你的OS滴答定时器初始化代码 **/
	__asm__ volatile("cpsid	i\n\t");     /*< 关中断 */
	SysTick_Config(PL_CFG_SYSTICK_TIME_SLICE_US * 72); // 1us 1900: 12.5us,  1800:25us,   3600:50us,   72000:1ms
	NVIC_EnableIRQ(SysTick_IRQn);
	__asm__ volatile("cpsie	i\n\t");     /*< 开中断 */
}


void pl_port_switch_context(void);

void SysTick_Handler(void);
void SysTick_Handler(void)
{
	USART1_PrintChar('R');
	__asm__ volatile("cpsid	i\n\t");     /*< 关中断 */
	pl_port_switch_context();
	__asm__ volatile("cpsie	i\n\t");     /*< 开中断 */
}

void pl_callee_save_curr_context_sp(void *context_sp);
void pl_callee_save_curr_context_sp(void *context_sp)
{
	USART1_PrintChar('T');
	TaskControlBlockArray[TaskID].TaskStackPointer = (uint32_t)context_sp;
}

void *pl_callee_get_next_context_sp(void);
void *pl_callee_get_next_context_sp(void)
{
	USART1_PrintChar('B');
	TaskID = (TaskID + 1) % TaskNumbers;
	return (uint32_t *)TaskControlBlockArray[TaskID].TaskStackPointer;
}

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
	uint_t USART1_BaudRate = 115200;
	USART1_Init(USART1_BaudRate);
	USART1_PrintChar('S');
	USART1_PrintChar('\t');
	TaskCreate(idle_task, IdleTaskStack, IdleTaskStackSize, &TaskControlBlockArray[0], (void *)'I');
	TaskCreate(task, Task01Stack, Task01StackSize, &TaskControlBlockArray[1], (void *)'1');
	TaskCreate(task, Task02Stack, Task02StackSize, &TaskControlBlockArray[2], (void *)'2');
	TaskCreate(task, Task03Stack, Task03StackSize, &TaskControlBlockArray[3], (void *)'3');
	pl_port_switch_context();
	while(1);

}

