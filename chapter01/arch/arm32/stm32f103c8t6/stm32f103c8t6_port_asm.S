/*===============================================================================================
 文件：rts_port.S
 说明：任务切换相关函数
 作者：lishanwu  (SillyMem)
 时间：2022.04.09
 附言：该文件针对STM32F103的RTS操作系统移植

===============================================================================================*/
#define SCB_ICSR_REG   0xE000ED04      //中断控制及状态寄存器，参考《Cortex-M3权威指南》P305
.extern pl_callee_get_next_context	   //pl是PlainOS项目代号缩写

.global PendSV_Handler
.global pl_port_switch_context
.global pl_port_cpu_dmb
.global pl_port_cpu_dsb
.global pl_port_cpu_isb


/*
在Thumb模式下，push/pop是单独的指令，具有自己的助记符和操作码。e.g. b500 push {lr}。
使用它是因为它更高效（当您的寄存器列表只包括r0..r7，lr中的寄存器时）
stmfd存在于Thumb-2中，但GAS显然只能在.syntax unified模式下正确处理它。
如果没有这一点，stmfd sp!,{lr}将给出一个Error: lo register required。
但是有了.syntax unified我们得到了f84d ed04 str.w lr, [sp, #-4]!，
再加上其他寄存器，我们可以做像e929 4803 stmdb r9!, {r0, r1, fp, lr}这样的事情，
它明确地使用non-lo寄存器作为地址和register-list中的stmdb助记符，
并反汇编为stmdb(FD=完全递减与DB=之前递减相同，用于stores.）
e、 我把这个放到foo.s中，这样我就可以在上面运行
arm-none-eabi-gcc -c(gcc -mthumb对于汇编来说似乎无关紧要，只是编译C）。
*/
///笔者注：这里可以直接使用pop和push指令(使用MSP)

.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb


.section .text.pl_port_switch_context
.type pl_port_switch_context, %function

pl_port_switch_context:

	ldr   r1, =SCB_ICSR_REG		//将中断控制及状态寄存器地址存入R1
	ldr   r2, [r1,#0]			//将中断控制及状态寄存器地址单元内容复制给R2寄存器存储
	ldr   r3, =0x10000000		//中断控制及状态寄存器PENDSTSET位置Write 1 to pend SYSTICK exception，不让滴答定时器工作，这里需要观察Exception Types表格参考The Definitive Guide to ARM Cortex M3 and Cortex M4 Processors, 3rd Edition Pe108页码
	orr   r2, r2, r3			//将中断控制及状态寄存器地址单元内容与0x10000000按位或预算复制给R2存储
	str   r2, [r1,#0]			//将R2内容复制给SCB_ICSR_REG地址单元存储
	bx    lr					//Branch with Exchange，BX LR instruction to perform the exception return，link register (LR) R14，其他程序调用完成后返回PC，PC由LR寄存器决定


.section .data
is_first_switch:
	.word 0x0					//is_first_switch位置放置0x0，占用一个word大小

.section .text.PendSV_Handler
.type PendSV_Handler, %function

PendSV_Handler:
	cpsid i /* Change Processor State (CPS) disbale interrupt */
	/* clear pending bit of PendSV */
	ldr   r1, =SCB_ICSR_REG			
pending status
	ldr   r2, [r1]					//将SCB_ICSR_REG内容给R2作为内容
	orr   r2, r2, #0x80000000		//NMIPENDSET Write 1 to pend NMI，Read vale indicates NMI，将不可屏蔽中断阻塞
	str   r2, [r1]					//将运算结果还给SCB_ICSR_REG作为内容

	ldr   r1, =is_first_switch		//R1将is_first_switch地址作为内容
	ldr   r2, [r1]					//将is_first_switch内容送给R2作为内容					
	ldr   r3, =0xff					//将0xff送给R3作为内容
	str   r3, [r1]					//将0xff送给is_first_switch内存单元作为内容
	cmp   r2, #0					//将is_first_switch存储单元内容和0相比
	beq   switch_bottom				//if Equal, then go to p2 switch_bottom

switch_top:
	/* save context */
	mrs   r0, psp					//将PSP内容放入到R0中
	stmdb r0!, {r4-r11}				//Write multiple words to memory location specified by Rd. Address Decrement Before each write Rn writes back after the transfer is done.
	push {lr}						//将LR寄存器存储的地址压入堆栈中,保存此时LR内容
	/* r0 = pl_callee_save_curr_context_sp(sp) */
	bl   pl_callee_save_curr_context_sp			//bl将此时PC地址放入LR寄存器中，然后跳到其他函数执行
	pop  {lr}									//还原调用pl_callee_save_curr_context_sp函数前的LR内容

switch_bottom:

	cpsie i  /* enable interrupt */
	push {lr}	//总结规律，函数调用与中断会干涉LR内容，所以LR内容需要保护现场与还原现场
	/* r0 = pl_callee_get_next_context() */
	bl   pl_callee_get_next_context
	pop  {lr}

	cpsid i  /* disable interrupt */
	/* restore context */
	ldmia r0!, {r4-r11}
	msr   psp, r0
	orr   lr, lr, #0x04
	cpsie i  /* enable interrupt */
	bx    lr

////////////// barrier implement //////////////////
.section .text.pl_port_cpu_dmb
.type pl_port_cpu_dmb, %function
pl_port_cpu_dmb:               		//数据隔离
	dmb 0xF
	bx  lr

.section .text.pl_port_cpu_dsb
.type pl_port_cpu_dsb, %function
pl_port_cpu_dsb:
	dsb 0xF
	bx  lr

.section .text.pl_port_cpu_isb
.type pl_port_cpu_isb, %function
pl_port_cpu_isb:
	isb 0xF
	bx  lr
