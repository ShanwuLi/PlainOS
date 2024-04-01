/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

.extern pl_callee_get_next_context_sp
.extern pl_callee_save_curr_context_sp

.global pl_port_rodata_read8
.global pl_port_rodata_read16
.global pl_port_rodata_read32
.global pl_port_rodata_read
.global pl_port_cpu_dmb
.global pl_port_cpu_dsb
.global pl_port_cpu_isb

.global pl_port_switch_context


.section .text, "ax"
.type pl_port_rodata_read8, %function
pl_port_rodata_read8:
	mov r30, r24
	mov r31, r25
	elpm r24, z
	ret

.section .text, "ax"
.type pl_port_rodata_read16, %function
pl_port_rodata_read16:
	mov r30, r24
	mov r31, r25
	elpm r24, z+
	elpm r25, z
	ret

.section .text, "ax"
.type pl_port_rodata_read32, %function
pl_port_rodata_read32:
	mov r30, r24
	mov r31, r25
	elpm r22, z+
	elpm r23, z+
	elpm r24, z+
	elpm r25, z
	ret

.section .text, "ax"
.type pl_port_rodata_read, %function
pl_port_rodata_read:
	jmp pl_port_rodata_read16


.section .data
once_entry:
	.byte 0x0

.section .text, "ax"
.type pl_port_switch_context, %function
pl_port_switch_context:

	lds   r30, once_entry
	cpi   r30, 0
	breq  set_onceentry_to_1

	push   r1        /*< R29入栈   */
	push   r0        /*< R28入栈   */
	lds    r0, 0x5f  /*< SREG入栈  */
	push   r0
	lds    r0, 0x5B  /*< RAMPZ入栈 */
	push   r0

	push   r2
	push   r3
	push   r4
	push   r5
	push   r6
	push   r7
	push   r8
	push   r9
	push   r10
 	push   r11
	push   r12
	push   r13
	push   r14
	push   r15
	push   r16
	push   r17
	push   r18
	push   r19
	push   r20
	push   r21
	push   r22
	push   r23
	push   r24
	push   r25
	push   r26
	push   r27
	push   r30
	push   r31
	push   r29
	push   r28

	lds    r24, 0x5d /* save sp_l */
	lds    r25, 0x5e /* save sp_h */
	call   pl_callee_save_curr_context_sp



set_onceentry_to_1:
	ldi r30, 1
	sts once_entry,  r30

switch_bottom:
	call pl_callee_get_next_context_sp
	sts  0x5d, r24 /* restore sp_l */
	sts  0x5e, r25 /* restore sp_h */
	pop  r28       /*< 各寄存器出栈，恢复任务环境   */
	pop  r29
	pop  r31
	pop  r30
	pop  r27
	pop  r26
	pop	 r25
	pop  r24
	pop  r23
	pop  r22
	pop  r21
	pop  r20
	pop  r19
	pop  r18
	pop  r17
	pop  r16
	pop  r15
	pop  r14
	pop  r13
	pop  r12
	pop  r11
	pop  r10
	pop  r9
	pop  r8
	pop  r7
	pop  r6
	pop  r5
	pop  r4
	pop  r3
	pop  r2
	pop  r0
	sts  0x5B, r0   /*< RAMPZ出栈    */
	pop  r0         /*< SREG出栈     */
	sts  0x5f, r0
	pop  r0
	pop	 r1
	ret


.section .text, "ax"
.type pl_port_cpu_dmb, %function
pl_port_cpu_dmb:
	ret


.section .text, "ax"
.type pl_port_cpu_dsb, %function
pl_port_cpu_dsb:
	ret


.section .text, "ax"
.type pl_port_cpu_isb, %function
pl_port_cpu_isb:
	ret
