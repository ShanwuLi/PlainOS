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
.global pl_port_rodata_read8
.global pl_port_rodata_read16
.global pl_port_rodata_read32
.global pl_port_rodata_read

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
