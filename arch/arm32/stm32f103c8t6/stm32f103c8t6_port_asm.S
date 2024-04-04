/*===============================================================================================
 文件：rts_port.S
 说明：任务切换相关函数
 作者：lishanwu  (SillyMem)
 时间：2022.04.09
 附言：该文件针对STM32F103的RTS操作系统移植

===============================================================================================*/
#define SCB_ICSR_REG   0xE000ED04
.extern pl_callee_get_next_context_sp
.extern pl_callee_save_curr_context_sp

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

	ldr   r1, =SCB_ICSR_REG
	ldr   r2, [r1,#0]
	ldr   r3, =0x10000000
	orr   r2, r2, r3
	str   r2, [r1,#0]
	bx    lr


.section .text.PendSV_Handler
.type PendSV_Handler, %function

PendSV_Handler:
	cpsid i /* disbale interrupt */
	/* clear pending bit of PendSV */
	ldr   r1, =SCB_ICSR_REG
	ldr   r2, [r1]
	orr   r2, r2, #0x80000000
	str   r2, [r1]

switch_top:
	/* save context */
	mrs   r0, psp
	stmdb r0!, {r4-r11}
	push {lr}
	/* r0 = pl_callee_save_curr_context_sp(sp) */
	bl   pl_callee_save_curr_context_sp
	pop  {lr}
	dmb 0xF

switch_bottom:

	cpsie i  /* enable interrupt */
	push {lr}
	/* r0 = pl_callee_get_next_context() */
	bl   pl_callee_get_next_context_sp
	pop  {lr}
	dmb 0xF

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
pl_port_cpu_dmb:
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
