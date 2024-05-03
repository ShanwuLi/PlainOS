# 第二章 任务管理
# 2.1 手动切换2个任务
手动实现2个任务的切换需要如下要素。
- 上下文切换需要保存现场，每个任务需要创建专属于自己的任务栈空间，任务栈空间的大小受到MCU切换需要保存的寄存器总数和任务自身需要保护的参数数量影响。
- 任务第一次创建时需要保存当前上下文，这个功能由任务创建函数实现，这个函数应在后续2.2章节的task.c中进行管理。
- 任务切换函数体在STM32中通常使用PendSV异常服务函数实现，任务切换函数功能应该包含保护现场和恢复现场两个核心功能。任务切换需要小心处理任务栈在保存现场和恢复现场时任务栈SP的位置变化，所以需要一个专门的变量来对MCU的SP进行记录。
```C
PendSV_Handler:
	cpsid i /* Change Processor State (CPS) disbale interrupt */
	/* clear pending bit of PendSV */
	ldr   r1, =SCB_ICSR_REG		
	ldr   r1, =is_first_switch		//R1将is_first_switch地址作为内容
	ldr   r2, [r1]					//将is_first_switch内容送给R2作为内容					
	ldr   r3, =0xff					//将0xff送给R3作为内容
	str   r3, [r1]					//将0xff送给is_first_switch内存单元作为内容
	cmp   r2, #0					//将is_first_switch存储单元内容和0相比
	beq   switch_bottom				//if Equal, then go to p2 switch_botto
switch_top:
	/* save context */
	mrs   r0, psp					//将PSP内容放入到R0中
	stmdb r0!, {r4-r11}				//Write multiple words to memory location specifieby Rd.      Address Decrement Before each write Rn writes back after the transfer idone.
	push {lr}						//将LR寄存器存储的地址压入堆栈中,保存此时LR内容
	/* r0 = pl_callee_save_curr_context_sp(sp) */
	bl   pl_callee_save_curr_context_sp			//bl将此时PC地址放入LR寄存器中，然后跳到函数执行
	pop  {lr}									//还原调用pl_callee_save_curr_context_s数前的LR      内
switch_bottom
	cpsie i  /* enable interrupt */
	push {lr}	//总结规律，函数调用与中断会干涉LR内容，所以LR内容需要保护现场与还原现场
	/* r0 = pl_callee_get_next_context() */
	bl   pl_callee_get_next_context_sp
	pop  {lr
	cpsid i  /* disable interrupt */
	/* restore context */
	ldmia r0!, {r4-r11}
	msr   psp, r0
	orr   lr, lr, #0x04
	cpsie i  /* enable interrupt */
	bx    lr
```
- 任务栈TaskStackPointer从MCU SP中获取位置与送入位置信息
```C
void pl_callee_save_curr_context_sp(void *context_sp);
void pl_callee_save_curr_context_sp(void *context_sp)
{
	TaskControlBlockArray[TaskID].TaskStackPointer = (uint32_t)context_sp;
}

void *pl_callee_get_next_context_sp(void);
void *pl_callee_get_next_context_sp(void)
{
	TaskID = (TaskID + 1) % 2;
	return (uint32_t *)TaskControlBlockArray[TaskID].TaskStackPointer;
}
```
# 2.1 手动切换N个任务
# 2.1 定时切换N个任务
# 2.1 定时切换N个任务