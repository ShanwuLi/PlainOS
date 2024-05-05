/*===============================================================================================
 文件：rts_port.S
 说明：任务切换相关函数
 作者：lishanwu  (SillyMem)
 时间：2022.04.09
 附言：该文件针对STM32F103的RTS操作系统移植

===============================================================================================*/
#define SCB_ICSR_REG   0xE000ED04      //0xE000ED04为Interrupt Control and State Register地址
.extern pl_callee_save_curr_context_sp //.extern声明外部因素后pl_callee_save_curr_context_sp外部因素对本文件可用
.extern pl_callee_get_next_context_sp

.global PendSV_Handler 				   //.global声明全局因素后，其他文件可以调用本文件中PendSV_Handler因素
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
.cpu cortex-m3	//Cortex-M3使用16bit thumb和32bit thumb2指令集，thumb指令集是thumb2指令集子集
.fpu softvfp
.thumb	


.section .text.pl_port_switch_context
.type pl_port_switch_context, %function

pl_port_switch_context:

	ldr   r1, =SCB_ICSR_REG		//ldr load something to register，将SCB_ICSR_REG装入r1
	ldr   r2, [r1,#0]			//将[SCB_ICSR_REG,#0]解引用内容给R2
	ldr   r3, =0x10000000		//SCB_ICSR_REG的28bit PENDSVSET位置1，主动触发 PENDSV异常
	orr   r2, r2, r3			//or something to register，将r2, r3按位或运算结果给R2
	str   r2, [r1,#0]			//store something to register，将R2内容存在[r1,#0]中
	bx    lr					//Branch with Exchange，其他函数调用pl_port_switch_context会将调用点PC存入LR，pl_port_switch_context执行结束，LR弹出值给PC返回到调用点重新顺序执行


.section .data
is_first_switch:
	.word 0x0					//is_first_switch位置放置0x0，占用一个word大小

.section .text.PendSV_Handler
.type PendSV_Handler, %function

PendSV_Handler:
	cpsid i /* Change Processor State (CPS) disbale interrupt */
	/* clear pending bit of PendSV PendSV异常处理完后一般硬件自动清零，这里一段也可以不写*/
	ldr   r1, =SCB_ICSR_REG
	ldr   r2, [r1,#0]
	ldr   r3, =0x08000000
	orr   r2, r2, r3
	str   r2, [r1,#0]
	/* clear pending bit of PendSV */

	ldr   r1, =is_first_switch		//将is_first_switch值给R1
	ldr   r2, [r1]					//将[is_first_switch]解引用内容0x0给R2
	ldr   r3, =0xff					//将0xff送给R3作为内容
	str   r3, [r1]					//将0xff送给[is_first_switch]作为内容
	cmp   r2, #0					//将is_first_switch存储单元内容和0相比
	beq   switch_bottom				//if Equal, then go to p2 switch_bottom

switch_top:
	/* save context */
	mrs   r0, psp					//move from PSP to R0
	stmdb r0!, {r4-r11}				//Store multiple registers to memory，decrement before
	push {lr}						//将LR寄存器存储的地址压入堆栈中,保存此时LR内容
	/* r0 = pl_callee_save_curr_context_sp(sp) */
	bl   pl_callee_save_curr_context_sp			//bl将此时PC地址放入LR寄存器中，然后跳到其他函数执行
	pop  {lr}									//还原调用pl_callee_save_curr_context_sp函数前的LR内容

switch_bottom:

	cpsie i  /* enable interrupt */
	push {lr}	//总结规律，函数调用与中断会干涉LR内容，所以LR内容需要保护现场与还原现场
	/* r0 = pl_callee_get_next_context() */
	bl   pl_callee_get_next_context_sp
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
pl_port_cpu_dmb:               		//data memory barrier
	dmb 0xF
	bx  lr

.section .text.pl_port_cpu_dsb
.type pl_port_cpu_dsb, %function
pl_port_cpu_dsb:					//Data Synchronization Barrier
	dsb 0xF
	bx  lr

.section .text.pl_port_cpu_isb
.type pl_port_cpu_isb, %function
pl_port_cpu_isb:
	isb 0xF							//Instruction Synchronization Barrier
	bx  lr
