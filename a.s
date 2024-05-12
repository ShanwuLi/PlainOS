
out/stm32f103c8t6.elf:     file format elf32-littlearm


Disassembly of section .text:

0800010c <__do_global_dtors_aux>:
 800010c:	b510      	push	{r4, lr}
 800010e:	4c05      	ldr	r4, [pc, #20]	; (8000124 <__do_global_dtors_aux+0x18>)
 8000110:	7823      	ldrb	r3, [r4, #0]
 8000112:	b933      	cbnz	r3, 8000122 <__do_global_dtors_aux+0x16>
 8000114:	4b04      	ldr	r3, [pc, #16]	; (8000128 <__do_global_dtors_aux+0x1c>)
 8000116:	b113      	cbz	r3, 800011e <__do_global_dtors_aux+0x12>
 8000118:	4804      	ldr	r0, [pc, #16]	; (800012c <__do_global_dtors_aux+0x20>)
 800011a:	f3af 8000 	nop.w
 800011e:	2301      	movs	r3, #1
 8000120:	7023      	strb	r3, [r4, #0]
 8000122:	bd10      	pop	{r4, pc}
 8000124:	20000004 	.word	0x20000004
 8000128:	00000000 	.word	0x00000000
 800012c:	0800010c 	.word	0x0800010c

08000130 <frame_dummy>:
 8000130:	b508      	push	{r3, lr}
 8000132:	4b03      	ldr	r3, [pc, #12]	; (8000140 <frame_dummy+0x10>)
 8000134:	b11b      	cbz	r3, 800013e <frame_dummy+0xe>
 8000136:	4903      	ldr	r1, [pc, #12]	; (8000144 <frame_dummy+0x14>)
 8000138:	4803      	ldr	r0, [pc, #12]	; (8000148 <frame_dummy+0x18>)
 800013a:	f3af 8000 	nop.w
 800013e:	bd08      	pop	{r3, pc}
 8000140:	00000000 	.word	0x00000000
 8000144:	20000008 	.word	0x20000008
 8000148:	0800010c 	.word	0x0800010c

0800014c <__aeabi_frsub>:
 800014c:	f080 4000 	eor.w	r0, r0, #2147483648	; 0x80000000
 8000150:	e002      	b.n	8000158 <__addsf3>
 8000152:	bf00      	nop

08000154 <__aeabi_fsub>:
 8000154:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000

08000158 <__addsf3>:
 8000158:	0042      	lsls	r2, r0, #1
 800015a:	bf1f      	itttt	ne
 800015c:	ea5f 0341 	movsne.w	r3, r1, lsl #1
 8000160:	ea92 0f03 	teqne	r2, r3
 8000164:	ea7f 6c22 	mvnsne.w	ip, r2, asr #24
 8000168:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 800016c:	d06a      	beq.n	8000244 <__addsf3+0xec>
 800016e:	ea4f 6212 	mov.w	r2, r2, lsr #24
 8000172:	ebd2 6313 	rsbs	r3, r2, r3, lsr #24
 8000176:	bfc1      	itttt	gt
 8000178:	18d2      	addgt	r2, r2, r3
 800017a:	4041      	eorgt	r1, r0
 800017c:	4048      	eorgt	r0, r1
 800017e:	4041      	eorgt	r1, r0
 8000180:	bfb8      	it	lt
 8000182:	425b      	neglt	r3, r3
 8000184:	2b19      	cmp	r3, #25
 8000186:	bf88      	it	hi
 8000188:	4770      	bxhi	lr
 800018a:	f010 4f00 	tst.w	r0, #2147483648	; 0x80000000
 800018e:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8000192:	f020 407f 	bic.w	r0, r0, #4278190080	; 0xff000000
 8000196:	bf18      	it	ne
 8000198:	4240      	negne	r0, r0
 800019a:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 800019e:	f441 0100 	orr.w	r1, r1, #8388608	; 0x800000
 80001a2:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
 80001a6:	bf18      	it	ne
 80001a8:	4249      	negne	r1, r1
 80001aa:	ea92 0f03 	teq	r2, r3
 80001ae:	d03f      	beq.n	8000230 <__addsf3+0xd8>
 80001b0:	f1a2 0201 	sub.w	r2, r2, #1
 80001b4:	fa41 fc03 	asr.w	ip, r1, r3
 80001b8:	eb10 000c 	adds.w	r0, r0, ip
 80001bc:	f1c3 0320 	rsb	r3, r3, #32
 80001c0:	fa01 f103 	lsl.w	r1, r1, r3
 80001c4:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 80001c8:	d502      	bpl.n	80001d0 <__addsf3+0x78>
 80001ca:	4249      	negs	r1, r1
 80001cc:	eb60 0040 	sbc.w	r0, r0, r0, lsl #1
 80001d0:	f5b0 0f00 	cmp.w	r0, #8388608	; 0x800000
 80001d4:	d313      	bcc.n	80001fe <__addsf3+0xa6>
 80001d6:	f1b0 7f80 	cmp.w	r0, #16777216	; 0x1000000
 80001da:	d306      	bcc.n	80001ea <__addsf3+0x92>
 80001dc:	0840      	lsrs	r0, r0, #1
 80001de:	ea4f 0131 	mov.w	r1, r1, rrx
 80001e2:	f102 0201 	add.w	r2, r2, #1
 80001e6:	2afe      	cmp	r2, #254	; 0xfe
 80001e8:	d251      	bcs.n	800028e <__addsf3+0x136>
 80001ea:	f1b1 4f00 	cmp.w	r1, #2147483648	; 0x80000000
 80001ee:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 80001f2:	bf08      	it	eq
 80001f4:	f020 0001 	biceq.w	r0, r0, #1
 80001f8:	ea40 0003 	orr.w	r0, r0, r3
 80001fc:	4770      	bx	lr
 80001fe:	0049      	lsls	r1, r1, #1
 8000200:	eb40 0000 	adc.w	r0, r0, r0
 8000204:	3a01      	subs	r2, #1
 8000206:	bf28      	it	cs
 8000208:	f5b0 0f00 	cmpcs.w	r0, #8388608	; 0x800000
 800020c:	d2ed      	bcs.n	80001ea <__addsf3+0x92>
 800020e:	fab0 fc80 	clz	ip, r0
 8000212:	f1ac 0c08 	sub.w	ip, ip, #8
 8000216:	ebb2 020c 	subs.w	r2, r2, ip
 800021a:	fa00 f00c 	lsl.w	r0, r0, ip
 800021e:	bfaa      	itet	ge
 8000220:	eb00 50c2 	addge.w	r0, r0, r2, lsl #23
 8000224:	4252      	neglt	r2, r2
 8000226:	4318      	orrge	r0, r3
 8000228:	bfbc      	itt	lt
 800022a:	40d0      	lsrlt	r0, r2
 800022c:	4318      	orrlt	r0, r3
 800022e:	4770      	bx	lr
 8000230:	f092 0f00 	teq	r2, #0
 8000234:	f481 0100 	eor.w	r1, r1, #8388608	; 0x800000
 8000238:	bf06      	itte	eq
 800023a:	f480 0000 	eoreq.w	r0, r0, #8388608	; 0x800000
 800023e:	3201      	addeq	r2, #1
 8000240:	3b01      	subne	r3, #1
 8000242:	e7b5      	b.n	80001b0 <__addsf3+0x58>
 8000244:	ea4f 0341 	mov.w	r3, r1, lsl #1
 8000248:	ea7f 6c22 	mvns.w	ip, r2, asr #24
 800024c:	bf18      	it	ne
 800024e:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 8000252:	d021      	beq.n	8000298 <__addsf3+0x140>
 8000254:	ea92 0f03 	teq	r2, r3
 8000258:	d004      	beq.n	8000264 <__addsf3+0x10c>
 800025a:	f092 0f00 	teq	r2, #0
 800025e:	bf08      	it	eq
 8000260:	4608      	moveq	r0, r1
 8000262:	4770      	bx	lr
 8000264:	ea90 0f01 	teq	r0, r1
 8000268:	bf1c      	itt	ne
 800026a:	2000      	movne	r0, #0
 800026c:	4770      	bxne	lr
 800026e:	f012 4f7f 	tst.w	r2, #4278190080	; 0xff000000
 8000272:	d104      	bne.n	800027e <__addsf3+0x126>
 8000274:	0040      	lsls	r0, r0, #1
 8000276:	bf28      	it	cs
 8000278:	f040 4000 	orrcs.w	r0, r0, #2147483648	; 0x80000000
 800027c:	4770      	bx	lr
 800027e:	f112 7200 	adds.w	r2, r2, #33554432	; 0x2000000
 8000282:	bf3c      	itt	cc
 8000284:	f500 0000 	addcc.w	r0, r0, #8388608	; 0x800000
 8000288:	4770      	bxcc	lr
 800028a:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 800028e:	f043 40fe 	orr.w	r0, r3, #2130706432	; 0x7f000000
 8000292:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8000296:	4770      	bx	lr
 8000298:	ea7f 6222 	mvns.w	r2, r2, asr #24
 800029c:	bf16      	itet	ne
 800029e:	4608      	movne	r0, r1
 80002a0:	ea7f 6323 	mvnseq.w	r3, r3, asr #24
 80002a4:	4601      	movne	r1, r0
 80002a6:	0242      	lsls	r2, r0, #9
 80002a8:	bf06      	itte	eq
 80002aa:	ea5f 2341 	movseq.w	r3, r1, lsl #9
 80002ae:	ea90 0f01 	teqeq	r0, r1
 80002b2:	f440 0080 	orrne.w	r0, r0, #4194304	; 0x400000
 80002b6:	4770      	bx	lr

080002b8 <__aeabi_ui2f>:
 80002b8:	f04f 0300 	mov.w	r3, #0
 80002bc:	e004      	b.n	80002c8 <__aeabi_i2f+0x8>
 80002be:	bf00      	nop

080002c0 <__aeabi_i2f>:
 80002c0:	f010 4300 	ands.w	r3, r0, #2147483648	; 0x80000000
 80002c4:	bf48      	it	mi
 80002c6:	4240      	negmi	r0, r0
 80002c8:	ea5f 0c00 	movs.w	ip, r0
 80002cc:	bf08      	it	eq
 80002ce:	4770      	bxeq	lr
 80002d0:	f043 4396 	orr.w	r3, r3, #1258291200	; 0x4b000000
 80002d4:	4601      	mov	r1, r0
 80002d6:	f04f 0000 	mov.w	r0, #0
 80002da:	e01c      	b.n	8000316 <__aeabi_l2f+0x2a>

080002dc <__aeabi_ul2f>:
 80002dc:	ea50 0201 	orrs.w	r2, r0, r1
 80002e0:	bf08      	it	eq
 80002e2:	4770      	bxeq	lr
 80002e4:	f04f 0300 	mov.w	r3, #0
 80002e8:	e00a      	b.n	8000300 <__aeabi_l2f+0x14>
 80002ea:	bf00      	nop

080002ec <__aeabi_l2f>:
 80002ec:	ea50 0201 	orrs.w	r2, r0, r1
 80002f0:	bf08      	it	eq
 80002f2:	4770      	bxeq	lr
 80002f4:	f011 4300 	ands.w	r3, r1, #2147483648	; 0x80000000
 80002f8:	d502      	bpl.n	8000300 <__aeabi_l2f+0x14>
 80002fa:	4240      	negs	r0, r0
 80002fc:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 8000300:	ea5f 0c01 	movs.w	ip, r1
 8000304:	bf02      	ittt	eq
 8000306:	4684      	moveq	ip, r0
 8000308:	4601      	moveq	r1, r0
 800030a:	2000      	moveq	r0, #0
 800030c:	f043 43b6 	orr.w	r3, r3, #1526726656	; 0x5b000000
 8000310:	bf08      	it	eq
 8000312:	f1a3 5380 	subeq.w	r3, r3, #268435456	; 0x10000000
 8000316:	f5a3 0300 	sub.w	r3, r3, #8388608	; 0x800000
 800031a:	fabc f28c 	clz	r2, ip
 800031e:	3a08      	subs	r2, #8
 8000320:	eba3 53c2 	sub.w	r3, r3, r2, lsl #23
 8000324:	db10      	blt.n	8000348 <__aeabi_l2f+0x5c>
 8000326:	fa01 fc02 	lsl.w	ip, r1, r2
 800032a:	4463      	add	r3, ip
 800032c:	fa00 fc02 	lsl.w	ip, r0, r2
 8000330:	f1c2 0220 	rsb	r2, r2, #32
 8000334:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 8000338:	fa20 f202 	lsr.w	r2, r0, r2
 800033c:	eb43 0002 	adc.w	r0, r3, r2
 8000340:	bf08      	it	eq
 8000342:	f020 0001 	biceq.w	r0, r0, #1
 8000346:	4770      	bx	lr
 8000348:	f102 0220 	add.w	r2, r2, #32
 800034c:	fa01 fc02 	lsl.w	ip, r1, r2
 8000350:	f1c2 0220 	rsb	r2, r2, #32
 8000354:	ea50 004c 	orrs.w	r0, r0, ip, lsl #1
 8000358:	fa21 f202 	lsr.w	r2, r1, r2
 800035c:	eb43 0002 	adc.w	r0, r3, r2
 8000360:	bf08      	it	eq
 8000362:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 8000366:	4770      	bx	lr

08000368 <__aeabi_f2uiz>:
 8000368:	0042      	lsls	r2, r0, #1
 800036a:	d20e      	bcs.n	800038a <__aeabi_f2uiz+0x22>
 800036c:	f1b2 4ffe 	cmp.w	r2, #2130706432	; 0x7f000000
 8000370:	d30b      	bcc.n	800038a <__aeabi_f2uiz+0x22>
 8000372:	f04f 039e 	mov.w	r3, #158	; 0x9e
 8000376:	ebb3 6212 	subs.w	r2, r3, r2, lsr #24
 800037a:	d409      	bmi.n	8000390 <__aeabi_f2uiz+0x28>
 800037c:	ea4f 2300 	mov.w	r3, r0, lsl #8
 8000380:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
 8000384:	fa23 f002 	lsr.w	r0, r3, r2
 8000388:	4770      	bx	lr
 800038a:	f04f 0000 	mov.w	r0, #0
 800038e:	4770      	bx	lr
 8000390:	f112 0f61 	cmn.w	r2, #97	; 0x61
 8000394:	d101      	bne.n	800039a <__aeabi_f2uiz+0x32>
 8000396:	0242      	lsls	r2, r0, #9
 8000398:	d102      	bne.n	80003a0 <__aeabi_f2uiz+0x38>
 800039a:	f04f 30ff 	mov.w	r0, #4294967295
 800039e:	4770      	bx	lr
 80003a0:	f04f 0000 	mov.w	r0, #0
 80003a4:	4770      	bx	lr
 80003a6:	bf00      	nop

080003a8 <__libc_init_array>:
 80003a8:	b570      	push	{r4, r5, r6, lr}
 80003aa:	2600      	movs	r6, #0
 80003ac:	4d0c      	ldr	r5, [pc, #48]	; (80003e0 <__libc_init_array+0x38>)
 80003ae:	4c0d      	ldr	r4, [pc, #52]	; (80003e4 <__libc_init_array+0x3c>)
 80003b0:	1b64      	subs	r4, r4, r5
 80003b2:	10a4      	asrs	r4, r4, #2
 80003b4:	42a6      	cmp	r6, r4
 80003b6:	d109      	bne.n	80003cc <__libc_init_array+0x24>
 80003b8:	f000 fa2a 	bl	8000810 <_init>
 80003bc:	2600      	movs	r6, #0
 80003be:	4d0a      	ldr	r5, [pc, #40]	; (80003e8 <__libc_init_array+0x40>)
 80003c0:	4c0a      	ldr	r4, [pc, #40]	; (80003ec <__libc_init_array+0x44>)
 80003c2:	1b64      	subs	r4, r4, r5
 80003c4:	10a4      	asrs	r4, r4, #2
 80003c6:	42a6      	cmp	r6, r4
 80003c8:	d105      	bne.n	80003d6 <__libc_init_array+0x2e>
 80003ca:	bd70      	pop	{r4, r5, r6, pc}
 80003cc:	f855 3b04 	ldr.w	r3, [r5], #4
 80003d0:	4798      	blx	r3
 80003d2:	3601      	adds	r6, #1
 80003d4:	e7ee      	b.n	80003b4 <__libc_init_array+0xc>
 80003d6:	f855 3b04 	ldr.w	r3, [r5], #4
 80003da:	4798      	blx	r3
 80003dc:	3601      	adds	r6, #1
 80003de:	e7f2      	b.n	80003c6 <__libc_init_array+0x1e>
 80003e0:	08000828 	.word	0x08000828
 80003e4:	08000828 	.word	0x08000828
 80003e8:	08000828 	.word	0x08000828
 80003ec:	0800082c 	.word	0x0800082c

080003f0 <SetSysClockTo72>:
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
static void SetSysClockTo72(void)
{
 80003f0:	b082      	sub	sp, #8
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
 80003f2:	2300      	movs	r3, #0
 80003f4:	9301      	str	r3, [sp, #4]
 80003f6:	9300      	str	r3, [sp, #0]
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 80003f8:	4a2c      	ldr	r2, [pc, #176]	; (80004ac <SetSysClockTo72+0xbc>)
 80003fa:	6813      	ldr	r3, [r2, #0]
 80003fc:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
 8000400:	6013      	str	r3, [r2, #0]
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
 8000402:	4b2a      	ldr	r3, [pc, #168]	; (80004ac <SetSysClockTo72+0xbc>)
 8000404:	681b      	ldr	r3, [r3, #0]
 8000406:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
 800040a:	9300      	str	r3, [sp, #0]
    StartUpCounter++;  
 800040c:	9b01      	ldr	r3, [sp, #4]
 800040e:	3301      	adds	r3, #1
 8000410:	9301      	str	r3, [sp, #4]
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));
 8000412:	9b00      	ldr	r3, [sp, #0]
 8000414:	b91b      	cbnz	r3, 800041e <SetSysClockTo72+0x2e>
 8000416:	9b01      	ldr	r3, [sp, #4]
 8000418:	f5b3 6fa0 	cmp.w	r3, #1280	; 0x500
 800041c:	d1f1      	bne.n	8000402 <SetSysClockTo72+0x12>

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
 800041e:	4b23      	ldr	r3, [pc, #140]	; (80004ac <SetSysClockTo72+0xbc>)
 8000420:	681b      	ldr	r3, [r3, #0]
 8000422:	f413 3f00 	tst.w	r3, #131072	; 0x20000
 8000426:	d006      	beq.n	8000436 <SetSysClockTo72+0x46>
  {
    HSEStatus = (uint32_t)0x01;
 8000428:	2301      	movs	r3, #1
 800042a:	9300      	str	r3, [sp, #0]
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
 800042c:	9b00      	ldr	r3, [sp, #0]
 800042e:	2b01      	cmp	r3, #1
 8000430:	d004      	beq.n	800043c <SetSysClockTo72+0x4c>
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
  }
}
 8000432:	b002      	add	sp, #8
 8000434:	4770      	bx	lr
    HSEStatus = (uint32_t)0x00;
 8000436:	2300      	movs	r3, #0
 8000438:	9300      	str	r3, [sp, #0]
 800043a:	e7f7      	b.n	800042c <SetSysClockTo72+0x3c>
    FLASH->ACR |= FLASH_ACR_PRFTBE;
 800043c:	4b1c      	ldr	r3, [pc, #112]	; (80004b0 <SetSysClockTo72+0xc0>)
 800043e:	681a      	ldr	r2, [r3, #0]
 8000440:	f042 0210 	orr.w	r2, r2, #16
 8000444:	601a      	str	r2, [r3, #0]
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
 8000446:	681a      	ldr	r2, [r3, #0]
 8000448:	f022 0203 	bic.w	r2, r2, #3
 800044c:	601a      	str	r2, [r3, #0]
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    
 800044e:	681a      	ldr	r2, [r3, #0]
 8000450:	f042 0202 	orr.w	r2, r2, #2
 8000454:	601a      	str	r2, [r3, #0]
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
 8000456:	f5a3 5380 	sub.w	r3, r3, #4096	; 0x1000
 800045a:	685a      	ldr	r2, [r3, #4]
 800045c:	605a      	str	r2, [r3, #4]
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
 800045e:	685a      	ldr	r2, [r3, #4]
 8000460:	605a      	str	r2, [r3, #4]
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
 8000462:	685a      	ldr	r2, [r3, #4]
 8000464:	f442 6280 	orr.w	r2, r2, #1024	; 0x400
 8000468:	605a      	str	r2, [r3, #4]
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
 800046a:	685a      	ldr	r2, [r3, #4]
 800046c:	f422 127c 	bic.w	r2, r2, #4128768	; 0x3f0000
 8000470:	605a      	str	r2, [r3, #4]
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
 8000472:	685a      	ldr	r2, [r3, #4]
 8000474:	f442 12e8 	orr.w	r2, r2, #1900544	; 0x1d0000
 8000478:	605a      	str	r2, [r3, #4]
    RCC->CR |= RCC_CR_PLLON;
 800047a:	681a      	ldr	r2, [r3, #0]
 800047c:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
 8000480:	601a      	str	r2, [r3, #0]
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
 8000482:	4b0a      	ldr	r3, [pc, #40]	; (80004ac <SetSysClockTo72+0xbc>)
 8000484:	681b      	ldr	r3, [r3, #0]
 8000486:	f013 7f00 	tst.w	r3, #33554432	; 0x2000000
 800048a:	d0fa      	beq.n	8000482 <SetSysClockTo72+0x92>
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
 800048c:	4b07      	ldr	r3, [pc, #28]	; (80004ac <SetSysClockTo72+0xbc>)
 800048e:	685a      	ldr	r2, [r3, #4]
 8000490:	f022 0203 	bic.w	r2, r2, #3
 8000494:	605a      	str	r2, [r3, #4]
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    
 8000496:	685a      	ldr	r2, [r3, #4]
 8000498:	f042 0202 	orr.w	r2, r2, #2
 800049c:	605a      	str	r2, [r3, #4]
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
 800049e:	4b03      	ldr	r3, [pc, #12]	; (80004ac <SetSysClockTo72+0xbc>)
 80004a0:	685b      	ldr	r3, [r3, #4]
 80004a2:	f003 030c 	and.w	r3, r3, #12
 80004a6:	2b08      	cmp	r3, #8
 80004a8:	d1f9      	bne.n	800049e <SetSysClockTo72+0xae>
 80004aa:	e7c2      	b.n	8000432 <SetSysClockTo72+0x42>
 80004ac:	40021000 	.word	0x40021000
 80004b0:	40022000 	.word	0x40022000

080004b4 <SetSysClock>:
{
 80004b4:	b508      	push	{r3, lr}
  SetSysClockTo72();
 80004b6:	f7ff ff9b 	bl	80003f0 <SetSysClockTo72>
}
 80004ba:	bd08      	pop	{r3, pc}

080004bc <SystemInit>:
{
 80004bc:	b508      	push	{r3, lr}
  RCC->CR |= (uint32_t)0x00000001;
 80004be:	4b10      	ldr	r3, [pc, #64]	; (8000500 <SystemInit+0x44>)
 80004c0:	681a      	ldr	r2, [r3, #0]
 80004c2:	f042 0201 	orr.w	r2, r2, #1
 80004c6:	601a      	str	r2, [r3, #0]
  RCC->CFGR &= (uint32_t)0xF8FF0000;
 80004c8:	6859      	ldr	r1, [r3, #4]
 80004ca:	4a0e      	ldr	r2, [pc, #56]	; (8000504 <SystemInit+0x48>)
 80004cc:	400a      	ands	r2, r1
 80004ce:	605a      	str	r2, [r3, #4]
  RCC->CR &= (uint32_t)0xFEF6FFFF;
 80004d0:	681a      	ldr	r2, [r3, #0]
 80004d2:	f022 7284 	bic.w	r2, r2, #17301504	; 0x1080000
 80004d6:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
 80004da:	601a      	str	r2, [r3, #0]
  RCC->CR &= (uint32_t)0xFFFBFFFF;
 80004dc:	681a      	ldr	r2, [r3, #0]
 80004de:	f422 2280 	bic.w	r2, r2, #262144	; 0x40000
 80004e2:	601a      	str	r2, [r3, #0]
  RCC->CFGR &= (uint32_t)0xFF80FFFF;
 80004e4:	685a      	ldr	r2, [r3, #4]
 80004e6:	f422 02fe 	bic.w	r2, r2, #8323072	; 0x7f0000
 80004ea:	605a      	str	r2, [r3, #4]
  RCC->CIR = 0x009F0000;
 80004ec:	f44f 021f 	mov.w	r2, #10420224	; 0x9f0000
 80004f0:	609a      	str	r2, [r3, #8]
  SetSysClock();
 80004f2:	f7ff ffdf 	bl	80004b4 <SetSysClock>
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
 80004f6:	4b04      	ldr	r3, [pc, #16]	; (8000508 <SystemInit+0x4c>)
 80004f8:	f04f 6200 	mov.w	r2, #134217728	; 0x8000000
 80004fc:	609a      	str	r2, [r3, #8]
}
 80004fe:	bd08      	pop	{r3, pc}
 8000500:	40021000 	.word	0x40021000
 8000504:	f8ff0000 	.word	0xf8ff0000
 8000508:	e000ed00 	.word	0xe000ed00

0800050c <NMI_Handler>:
  * @retval None
  */
void NMI_Handler(void)
{
	
}
 800050c:	4770      	bx	lr

0800050e <HardFault_Handler>:
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
 800050e:	e7fe      	b.n	800050e <HardFault_Handler>

08000510 <MemManage_Handler>:
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
 8000510:	e7fe      	b.n	8000510 <MemManage_Handler>

08000512 <BusFault_Handler>:
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
 8000512:	e7fe      	b.n	8000512 <BusFault_Handler>

08000514 <UsageFault_Handler>:
  */
void UsageFault_Handler(void)
{

  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
 8000514:	e7fe      	b.n	8000514 <UsageFault_Handler>

08000516 <SVC_Handler>:
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}
 8000516:	4770      	bx	lr

08000518 <DebugMon_Handler>:
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}
 8000518:	4770      	bx	lr
	...

0800051c <USART1_PrintChar>:
	USART1_PrintChar('S');
}

int USART1_PrintChar(char c)
{
	USART1->DR = c;
 800051c:	4b04      	ldr	r3, [pc, #16]	; (8000530 <USART1_PrintChar+0x14>)
 800051e:	8098      	strh	r0, [r3, #4]
	while(1) {
		if(USART1->SR & (1 << 6)) //USART_SR第六位表示Transmission Complete
 8000520:	4b03      	ldr	r3, [pc, #12]	; (8000530 <USART1_PrintChar+0x14>)
 8000522:	881b      	ldrh	r3, [r3, #0]
 8000524:	f013 0f40 	tst.w	r3, #64	; 0x40
 8000528:	d0fa      	beq.n	8000520 <USART1_PrintChar+0x4>
			return 0;
	}

	return -1;
}
 800052a:	2000      	movs	r0, #0
 800052c:	4770      	bx	lr
 800052e:	bf00      	nop
 8000530:	40013800 	.word	0x40013800

08000534 <USART1_Init>:
{
 8000534:	b538      	push	{r3, r4, r5, lr}
	RCC->APB2ENR|=(1<<14)|(1<<2);   //将IOPA时钟和USART1外设时钟使能       
 8000536:	4b22      	ldr	r3, [pc, #136]	; (80005c0 <USART1_Init+0x8c>)
 8000538:	699a      	ldr	r2, [r3, #24]
 800053a:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
 800053e:	f042 0204 	orr.w	r2, r2, #4
 8000542:	619a      	str	r2, [r3, #24]
	RCC->APB2RSTR|=(1<<14);         //将USART1外设复位       
 8000544:	68da      	ldr	r2, [r3, #12]
 8000546:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
 800054a:	60da      	str	r2, [r3, #12]
	RCC->APB2RSTR&=(~(1<<14));      //将USART1外设复位状态解除            
 800054c:	68da      	ldr	r2, [r3, #12]
 800054e:	f422 4280 	bic.w	r2, r2, #16384	; 0x4000
 8000552:	60da      	str	r2, [r3, #12]
	GPIOA->CRH&=0XFFFFF00F;         //将GPIOA A9和A10引脚输出模式清零       
 8000554:	f5a3 3384 	sub.w	r3, r3, #67584	; 0x10800
 8000558:	685a      	ldr	r2, [r3, #4]
 800055a:	f422 627f 	bic.w	r2, r2, #4080	; 0xff0
 800055e:	605a      	str	r2, [r3, #4]
	GPIOA->CRH|=0X000008B0;         //将GPIOA A9设置为上拉/下拉输入模式，A10引脚设置为复用推挽输出50MHz时钟模式    
 8000560:	685a      	ldr	r2, [r3, #4]
 8000562:	f442 620b 	orr.w	r2, r2, #2224	; 0x8b0
 8000566:	605a      	str	r2, [r3, #4]
	USART1->CR1|=(1<<13);           //使能USART外设        
 8000568:	4d16      	ldr	r5, [pc, #88]	; (80005c4 <USART1_Init+0x90>)
 800056a:	89ab      	ldrh	r3, [r5, #12]
 800056c:	b29b      	uxth	r3, r3
 800056e:	f443 5300 	orr.w	r3, r3, #8192	; 0x2000
 8000572:	81ab      	strh	r3, [r5, #12]
	USART1->CR1|=(1<<2);            //USART接收被使能     
 8000574:	89ab      	ldrh	r3, [r5, #12]
 8000576:	b29b      	uxth	r3, r3
 8000578:	f043 0304 	orr.w	r3, r3, #4
 800057c:	81ab      	strh	r3, [r5, #12]
	USART1->CR1|=(1<<5);            //缓冲区非空中断使能      
 800057e:	89ab      	ldrh	r3, [r5, #12]
 8000580:	b29b      	uxth	r3, r3
 8000582:	f043 0320 	orr.w	r3, r3, #32
 8000586:	81ab      	strh	r3, [r5, #12]
	USART1->CR1|=(1<<3);            //USART发送被使能                    
 8000588:	89ab      	ldrh	r3, [r5, #12]
 800058a:	b29b      	uxth	r3, r3
 800058c:	f043 0308 	orr.w	r3, r3, #8
 8000590:	81ab      	strh	r3, [r5, #12]
	integer=72*1000*1000/(USART1_BaudRate*16);         
 8000592:	0100      	lsls	r0, r0, #4
 8000594:	4c0c      	ldr	r4, [pc, #48]	; (80005c8 <USART1_Init+0x94>)
 8000596:	fbb4 f4f0 	udiv	r4, r4, r0
	decimal=(float)(72*1000*1000/(USART1_BaudRate*16))-integer;   
 800059a:	4620      	mov	r0, r4
 800059c:	f7ff fe8c 	bl	80002b8 <__aeabi_ui2f>
 80005a0:	4601      	mov	r1, r0
 80005a2:	f7ff fdd7 	bl	8000154 <__aeabi_fsub>
	USART1->BRR=(integer<<4)|((unsigned int)decimal*16);
 80005a6:	0124      	lsls	r4, r4, #4
 80005a8:	b2a4      	uxth	r4, r4
 80005aa:	f7ff fedd 	bl	8000368 <__aeabi_f2uiz>
 80005ae:	ea44 1400 	orr.w	r4, r4, r0, lsl #4
 80005b2:	b2a4      	uxth	r4, r4
 80005b4:	812c      	strh	r4, [r5, #8]
	USART1_PrintChar('S');
 80005b6:	2053      	movs	r0, #83	; 0x53
 80005b8:	f7ff ffb0 	bl	800051c <USART1_PrintChar>
}
 80005bc:	bd38      	pop	{r3, r4, r5, pc}
 80005be:	bf00      	nop
 80005c0:	40021000 	.word	0x40021000
 80005c4:	40013800 	.word	0x40013800
 80005c8:	044aa200 	.word	0x044aa200

080005cc <idle_task>:
		//pl_port_switch_context();
	};
}

void idle_task(void *arg);
void idle_task(void *arg){
 80005cc:	b508      	push	{r3, lr}
 80005ce:	4604      	mov	r4, r0
	while(1){
		USART1_PrintChar('T');
 80005d0:	2054      	movs	r0, #84	; 0x54
 80005d2:	f7ff ffa3 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar('A');
 80005d6:	2041      	movs	r0, #65	; 0x41
 80005d8:	f7ff ffa0 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar('S');
 80005dc:	2053      	movs	r0, #83	; 0x53
 80005de:	f7ff ff9d 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar('K');
 80005e2:	204b      	movs	r0, #75	; 0x4b
 80005e4:	f7ff ff9a 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar((char)((uint32_t)arg));
 80005e8:	b2e0      	uxtb	r0, r4
 80005ea:	f7ff ff97 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar('\t');
 80005ee:	2009      	movs	r0, #9
 80005f0:	f7ff ff94 	bl	800051c <USART1_PrintChar>
	while(1){
 80005f4:	e7ec      	b.n	80005d0 <idle_task+0x4>

080005f6 <TaskCreate>:
	stack = stack + StackSize; //为了方便理解这里将任务栈存储空间设置为Nx32结构和STM32寄存器位数保持一样，确保SP入栈一次减少4,stack变量减少1
 80005f6:	eb01 0282 	add.w	r2, r1, r2, lsl #2
	*(--stack)  = (u32_t)(1<<24);  /* XPSR */ //这里入栈顺序和《Cortex-M3权威指南9.1中断/异常的响应序列》一样，注意软件入栈顺序和硬件底层流水线入栈有些不一样
 80005fa:	f04f 7180 	mov.w	r1, #16777216	; 0x1000000
 80005fe:	f842 1c04 	str.w	r1, [r2, #-4]
	*(--stack)  = (u32_t)Task;     /* PC ,将任务地址压栈*/
 8000602:	f842 0c08 	str.w	r0, [r2, #-8]
	*(--stack)  = (u32_t)0;        /* LR */
 8000606:	2100      	movs	r1, #0
 8000608:	f842 1c0c 	str.w	r1, [r2, #-12]
	*(--stack)  = (u32_t)0;        /* R12 */
 800060c:	f842 1c10 	str.w	r1, [r2, #-16]
	*(--stack)  = (u32_t)0;        /* R3 */
 8000610:	f842 1c14 	str.w	r1, [r2, #-20]
	*(--stack)  = (u32_t)0;        /* R2 */
 8000614:	f842 1c18 	str.w	r1, [r2, #-24]
	*(--stack)  = (u32_t)0;        /* R1 */
 8000618:	f842 1c1c 	str.w	r1, [r2, #-28]
	*(--stack)  = (u32_t)arg;      /* R0 */
 800061c:	9800      	ldr	r0, [sp, #0]
 800061e:	f842 0c20 	str.w	r0, [r2, #-32]
	*(--stack)  = (u32_t)0;        /* R4 */	//这里入栈顺序依照用户喜欢的顺序，这里就按照寄存器名称顺序入栈
 8000622:	f842 1c24 	str.w	r1, [r2, #-36]
	*(--stack)  = (u32_t)0;        /* R5 */
 8000626:	f842 1c28 	str.w	r1, [r2, #-40]
	*(--stack)  = (u32_t)0;        /* R6 */
 800062a:	f842 1c2c 	str.w	r1, [r2, #-44]
	*(--stack)  = (u32_t)0;        /* R7 */
 800062e:	f842 1c30 	str.w	r1, [r2, #-48]
	*(--stack)  = (u32_t)0;        /* R8 */
 8000632:	f842 1c34 	str.w	r1, [r2, #-52]
	*(--stack)  = (u32_t)0;        /* R9 */
 8000636:	f842 1c38 	str.w	r1, [r2, #-56]
	*(--stack)  = (u32_t)0;        /* R10 */
 800063a:	f842 1c3c 	str.w	r1, [r2, #-60]
	*(--stack)  = (u32_t)0;        /* R11 */
 800063e:	f1a2 0040 	sub.w	r0, r2, #64	; 0x40
 8000642:	f842 1c40 	str.w	r1, [r2, #-64]
	TaskControlBlock->TaskStackPointer = (uint32_t)stack; //使用全局变量TaskControlBlock->TaskStackPointer成员记录个任务栈SP位置
 8000646:	6018      	str	r0, [r3, #0]
}
 8000648:	4770      	bx	lr
	...

0800064c <pl_port_systick_init>:
	__asm__ volatile("cpsid	i\n\t");     /*< 关中断 */
 800064c:	b672      	cpsid	i
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
 800064e:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
 8000652:	4a06      	ldr	r2, [pc, #24]	; (800066c <pl_port_systick_init+0x20>)
 8000654:	615a      	str	r2, [r3, #20]
    SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
 8000656:	4a06      	ldr	r2, [pc, #24]	; (8000670 <pl_port_systick_init+0x24>)
 8000658:	21f0      	movs	r1, #240	; 0xf0
 800065a:	f882 1023 	strb.w	r1, [r2, #35]	; 0x23
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
 800065e:	2200      	movs	r2, #0
 8000660:	619a      	str	r2, [r3, #24]
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
 8000662:	2207      	movs	r2, #7
 8000664:	611a      	str	r2, [r3, #16]
	__asm__ volatile("cpsie	i\n\t");     /*< 开中断 */
 8000666:	b662      	cpsie	i
}
 8000668:	4770      	bx	lr
 800066a:	bf00      	nop
 800066c:	006ddcff 	.word	0x006ddcff
 8000670:	e000ed00 	.word	0xe000ed00

08000674 <task>:
void task(void *arg){
 8000674:	b508      	push	{r3, lr}
 8000676:	4604      	mov	r4, r0
	pl_port_systick_init();
 8000678:	f7ff ffe8 	bl	800064c <pl_port_systick_init>
		USART1_PrintChar('T');
 800067c:	2054      	movs	r0, #84	; 0x54
 800067e:	f7ff ff4d 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar('A');
 8000682:	2041      	movs	r0, #65	; 0x41
 8000684:	f7ff ff4a 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar('S');
 8000688:	2053      	movs	r0, #83	; 0x53
 800068a:	f7ff ff47 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar('K');
 800068e:	204b      	movs	r0, #75	; 0x4b
 8000690:	f7ff ff44 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar((char)((uint32_t)arg));
 8000694:	b2e0      	uxtb	r0, r4
 8000696:	f7ff ff41 	bl	800051c <USART1_PrintChar>
		USART1_PrintChar('\t');
 800069a:	2009      	movs	r0, #9
 800069c:	f7ff ff3e 	bl	800051c <USART1_PrintChar>
	while(1){
 80006a0:	e7ec      	b.n	800067c <task+0x8>

080006a2 <SysTick_Handler>:
{
 80006a2:	b508      	push	{r3, lr}
	USART1_PrintChar('G');
 80006a4:	2047      	movs	r0, #71	; 0x47
 80006a6:	f7ff ff39 	bl	800051c <USART1_PrintChar>
	pl_port_switch_context();
 80006aa:	f000 f87b 	bl	80007a4 <pl_port_switch_context>
}
 80006ae:	bd08      	pop	{r3, pc}

080006b0 <pl_callee_save_curr_context_sp>:
{
 80006b0:	b510      	push	{r4, lr}
 80006b2:	4604      	mov	r4, r0
	USART1_PrintChar('T');
 80006b4:	2054      	movs	r0, #84	; 0x54
 80006b6:	f7ff ff31 	bl	800051c <USART1_PrintChar>
	TaskControlBlockArray[TaskID].TaskStackPointer = (uint32_t)context_sp;
 80006ba:	4b03      	ldr	r3, [pc, #12]	; (80006c8 <pl_callee_save_curr_context_sp+0x18>)
 80006bc:	781b      	ldrb	r3, [r3, #0]
 80006be:	b2db      	uxtb	r3, r3
 80006c0:	4a02      	ldr	r2, [pc, #8]	; (80006cc <pl_callee_save_curr_context_sp+0x1c>)
 80006c2:	f842 4023 	str.w	r4, [r2, r3, lsl #2]
}
 80006c6:	bd10      	pop	{r4, pc}
 80006c8:	20000828 	.word	0x20000828
 80006cc:	20000820 	.word	0x20000820

080006d0 <pl_callee_get_next_context_sp>:
{
 80006d0:	b508      	push	{r3, lr}
	USART1_PrintChar('B');
 80006d2:	2042      	movs	r0, #66	; 0x42
 80006d4:	f7ff ff22 	bl	800051c <USART1_PrintChar>
	TaskID = (TaskID + 1) % TaskNumbers;
 80006d8:	4a07      	ldr	r2, [pc, #28]	; (80006f8 <pl_callee_get_next_context_sp+0x28>)
 80006da:	7813      	ldrb	r3, [r2, #0]
 80006dc:	3301      	adds	r3, #1
 80006de:	f003 0301 	and.w	r3, r3, #1
 80006e2:	bf48      	it	mi
 80006e4:	425b      	negmi	r3, r3
 80006e6:	b2db      	uxtb	r3, r3
 80006e8:	7013      	strb	r3, [r2, #0]
	return (uint32_t *)TaskControlBlockArray[TaskID].TaskStackPointer;
 80006ea:	7813      	ldrb	r3, [r2, #0]
 80006ec:	b2db      	uxtb	r3, r3
 80006ee:	4a03      	ldr	r2, [pc, #12]	; (80006fc <pl_callee_get_next_context_sp+0x2c>)
 80006f0:	f852 0023 	ldr.w	r0, [r2, r3, lsl #2]
}
 80006f4:	bd08      	pop	{r3, pc}
 80006f6:	bf00      	nop
 80006f8:	20000828 	.word	0x20000828
 80006fc:	20000820 	.word	0x20000820

08000700 <pl_callee_entry>:
		//pl_port_switch_context();
	};
}

void pl_callee_entry(void);
void pl_callee_entry(){
 8000700:	b510      	push	{r4, lr}
 8000702:	b082      	sub	sp, #8
	uint_t USART1_BaudRate = 115200;
	USART1_Init(USART1_BaudRate);
 8000704:	f44f 30e1 	mov.w	r0, #115200	; 0x1c200
 8000708:	f7ff ff14 	bl	8000534 <USART1_Init>
	USART1_PrintChar('S');
 800070c:	2053      	movs	r0, #83	; 0x53
 800070e:	f7ff ff05 	bl	800051c <USART1_PrintChar>
	USART1_PrintChar('\t');
 8000712:	2009      	movs	r0, #9
 8000714:	f7ff ff02 	bl	800051c <USART1_PrintChar>
	TaskCreate(idle_task, IdleTaskStack, IdleTaskStackSize, &TaskControlBlockArray[0], (void *)'I');
 8000718:	4c0a      	ldr	r4, [pc, #40]	; (8000744 <pl_callee_entry+0x44>)
 800071a:	2349      	movs	r3, #73	; 0x49
 800071c:	9300      	str	r3, [sp, #0]
 800071e:	4623      	mov	r3, r4
 8000720:	f44f 7280 	mov.w	r2, #256	; 0x100
 8000724:	4908      	ldr	r1, [pc, #32]	; (8000748 <pl_callee_entry+0x48>)
 8000726:	4809      	ldr	r0, [pc, #36]	; (800074c <pl_callee_entry+0x4c>)
 8000728:	f7ff ff65 	bl	80005f6 <TaskCreate>
	TaskCreate(task, Task01Stack, Task01StackSize, &TaskControlBlockArray[1], (void *)'1');
 800072c:	2331      	movs	r3, #49	; 0x31
 800072e:	9300      	str	r3, [sp, #0]
 8000730:	1d23      	adds	r3, r4, #4
 8000732:	f44f 7280 	mov.w	r2, #256	; 0x100
 8000736:	4906      	ldr	r1, [pc, #24]	; (8000750 <pl_callee_entry+0x50>)
 8000738:	4806      	ldr	r0, [pc, #24]	; (8000754 <pl_callee_entry+0x54>)
 800073a:	f7ff ff5c 	bl	80005f6 <TaskCreate>
	pl_port_switch_context();
 800073e:	f000 f831 	bl	80007a4 <pl_port_switch_context>
	while(1);
 8000742:	e7fe      	b.n	8000742 <pl_callee_entry+0x42>
 8000744:	20000820 	.word	0x20000820
 8000748:	20000020 	.word	0x20000020
 800074c:	080005cd 	.word	0x080005cd
 8000750:	20000420 	.word	0x20000420
 8000754:	08000675 	.word	0x08000675

08000758 <Reset_Handler>:
	.weak	Reset_Handler		// 弱符号定义可以强符号代替，默认强符号
	.type	Reset_Handler, %function //Mark the symbol as being a function name，将Reset_Handler设置为函数名称，https://sourceware.org/binutils/docs/as/Type.html
Reset_Handler:

/* Copy the data segment initializers from flash to SRAM */
  movs	r1, #0
 8000758:	2100      	movs	r1, #0
  b	LoopCopyDataInit
 800075a:	e003      	b.n	8000764 <LoopCopyDataInit>

0800075c <CopyDataInit>:

CopyDataInit: 					//只是一个标号不需要定义直接使用
	ldr	r3, =_sidata
 800075c:	4b0b      	ldr	r3, [pc, #44]	; (800078c <LoopFillZerobss+0x14>)
	ldr	r3, [r3, r1]
 800075e:	585b      	ldr	r3, [r3, r1]
	str	r3, [r0, r1]
 8000760:	5043      	str	r3, [r0, r1]
	adds	r1, r1, #4
 8000762:	3104      	adds	r1, #4

08000764 <LoopCopyDataInit>:

LoopCopyDataInit:
	ldr	r0, =_sdata
 8000764:	480a      	ldr	r0, [pc, #40]	; (8000790 <LoopFillZerobss+0x18>)
	ldr	r3, =_edata
 8000766:	4b0b      	ldr	r3, [pc, #44]	; (8000794 <LoopFillZerobss+0x1c>)
	adds	r2, r0, r1
 8000768:	1842      	adds	r2, r0, r1
	cmp	r2, r3
 800076a:	429a      	cmp	r2, r3
	bcc	CopyDataInit
 800076c:	d3f6      	bcc.n	800075c <CopyDataInit>
	ldr	r2, =_sbss
 800076e:	4a0a      	ldr	r2, [pc, #40]	; (8000798 <LoopFillZerobss+0x20>)
	b	LoopFillZerobss
 8000770:	e002      	b.n	8000778 <LoopFillZerobss>

08000772 <FillZerobss>:
/* Zero fill the bss segment. */
FillZerobss:
	movs	r3, #0
 8000772:	2300      	movs	r3, #0
	str	r3, [r2], #4
 8000774:	f842 3b04 	str.w	r3, [r2], #4

08000778 <LoopFillZerobss>:

LoopFillZerobss:
	ldr	r3, = _ebss
 8000778:	4b08      	ldr	r3, [pc, #32]	; (800079c <LoopFillZerobss+0x24>)
	cmp	r2, r3
 800077a:	429a      	cmp	r2, r3
	bcc	FillZerobss
 800077c:	d3f9      	bcc.n	8000772 <FillZerobss>

/* Call the clock system intitialization function.*/
    bl  SystemInit
 800077e:	f7ff fe9d 	bl	80004bc <SystemInit>
/* Call static constructors */
    bl __libc_init_array
 8000782:	f7ff fe11 	bl	80003a8 <__libc_init_array>
/* Call the application's entry point.*/
	bl	pl_callee_entry
 8000786:	f7ff ffbb 	bl	8000700 <pl_callee_entry>
	bx	lr
 800078a:	4770      	bx	lr
	ldr	r3, =_sidata
 800078c:	08000830 	.word	0x08000830
	ldr	r0, =_sdata
 8000790:	20000000 	.word	0x20000000
	ldr	r3, =_edata
 8000794:	20000004 	.word	0x20000004
	ldr	r2, =_sbss
 8000798:	20000004 	.word	0x20000004
	ldr	r3, = _ebss
 800079c:	2000082c 	.word	0x2000082c

080007a0 <ADC1_2_IRQHandler>:
*/
  .section	.text.Default_Handler,"ax",%progbits //Only %S is supported at the moment, and substitutes the current section name. 
											     @Note on targets where the @ character is the start of a comment (eg ARM) then another character is used instead. For example the ARM port uses the % character.
Default_Handler:
Infinite_Loop:
	b	Infinite_Loop
 80007a0:	e7fe      	b.n	80007a0 <ADC1_2_IRQHandler>
	...

080007a4 <pl_port_switch_context>:
.section .text.pl_port_switch_context
.type pl_port_switch_context, %function

pl_port_switch_context:

	ldr   r1, =SCB_ICSR_REG		//ldr load something to register，将SCB_ICSR_REG装入r1
 80007a4:	4903      	ldr	r1, [pc, #12]	; (80007b4 <pl_port_switch_context+0x10>)
	ldr   r2, [r1,#0]			//将[SCB_ICSR_REG,#0]解引用内容给R2
 80007a6:	680a      	ldr	r2, [r1, #0]
	ldr   r3, =0x10000000		//SCB_ICSR_REG的28bit PENDSVSET位置1，主动触发 PENDSV异常
 80007a8:	f04f 5380 	mov.w	r3, #268435456	; 0x10000000
	orr   r2, r2, r3			//or something to register，将r2, r3按位或运算结果给R2
 80007ac:	ea42 0203 	orr.w	r2, r2, r3
	str   r2, [r1,#0]			//store something to register，将R2内容存在[r1,#0]中
 80007b0:	600a      	str	r2, [r1, #0]
	bx    lr					//Branch with Exchange，其他函数调用pl_port_switch_context会将调用点PC存入LR，pl_port_switch_context执行结束，LR弹出值给PC返回到调用点重新顺序执行
 80007b2:	4770      	bx	lr
	ldr   r1, =SCB_ICSR_REG		//ldr load something to register，将SCB_ICSR_REG装入r1
 80007b4:	e000ed04 	.word	0xe000ed04

080007b8 <PendSV_Handler>:

.section .text.PendSV_Handler
.type PendSV_Handler, %function

PendSV_Handler:
	cpsid i /* Change Processor State (CPS) disbale interrupt */
 80007b8:	b672      	cpsid	i
	/* clear pending bit of PendSV PendSV异常处理完后一般硬件自动清零，这里一段也可以不写*/
	ldr   r1, =SCB_ICSR_REG
 80007ba:	4913      	ldr	r1, [pc, #76]	; (8000808 <switch_bottom+0x20>)
	ldr   r2, [r1,#0]
 80007bc:	680a      	ldr	r2, [r1, #0]
	ldr   r3, =0x08000000
 80007be:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
	orr   r2, r2, r3
 80007c2:	ea42 0203 	orr.w	r2, r2, r3
	str   r2, [r1,#0]
 80007c6:	600a      	str	r2, [r1, #0]
	/* clear pending bit of PendSV */

	ldr   r1, =is_first_switch		//将is_first_switch值给R1
 80007c8:	4910      	ldr	r1, [pc, #64]	; (800080c <switch_bottom+0x24>)
	ldr   r2, [r1]					//将[is_first_switch]解引用内容0x0给R2
 80007ca:	680a      	ldr	r2, [r1, #0]
	ldr   r3, =0xff					//将0xff送给R3作为内容
 80007cc:	f04f 03ff 	mov.w	r3, #255	; 0xff
	str   r3, [r1]					//将0xff送给[is_first_switch]作为内容
 80007d0:	600b      	str	r3, [r1, #0]
	cmp   r2, #0					//将is_first_switch存储单元内容和0相比
 80007d2:	2a00      	cmp	r2, #0
	beq   switch_bottom				//if Equal, then go to p2 switch_bottom
 80007d4:	d008      	beq.n	80007e8 <switch_bottom>

080007d6 <switch_top>:

switch_top:
	/* save context */
	mrs   r0, psp					//move from PSP to R0
 80007d6:	f3ef 8009 	mrs	r0, PSP
	stmdb r0!, {r4-r11}				//Store multiple registers to memory，decrement before,STMDB Rn!,<reg list>
 80007da:	e920 0ff0 	stmdb	r0!, {r4, r5, r6, r7, r8, r9, sl, fp}
	push {lr}						//将LR寄存器存储的地址压入堆栈中,保存此时LR内容
 80007de:	b500      	push	{lr}
	/* pl_callee_save_curr_context_sp(sp) */
	bl   pl_callee_save_curr_context_sp			//bl将此时PC地址放入LR寄存器中，然后跳到其他函数执行
 80007e0:	f7ff ff66 	bl	80006b0 <pl_callee_save_curr_context_sp>
	pop  {lr}									//还原调用pl_callee_save_curr_context_sp函数前的LR内容
 80007e4:	f85d eb04 	ldr.w	lr, [sp], #4

080007e8 <switch_bottom>:

switch_bottom:
	/* resume context */
	cpsie i  /* enable interrupt */
 80007e8:	b662      	cpsie	i
	push {lr}	//总结规律，函数调用与中断会干涉LR内容，所以LR内容需要保护现场与还原现场
 80007ea:	b500      	push	{lr}
	/* r0 = pl_callee_get_next_context() */
	bl   pl_callee_get_next_context_sp
 80007ec:	f7ff ff70 	bl	80006d0 <pl_callee_get_next_context_sp>
	pop  {lr}
 80007f0:	f85d eb04 	ldr.w	lr, [sp], #4

	cpsid i  /* disable interrupt */
 80007f4:	b672      	cpsid	i
	/* restore context */
	ldmia r0!, {r4-r11}
 80007f6:	e8b0 0ff0 	ldmia.w	r0!, {r4, r5, r6, r7, r8, r9, sl, fp}
	msr   psp, r0
 80007fa:	f380 8809 	msr	PSP, r0
	orr   lr, lr, #0x04
 80007fe:	f04e 0e04 	orr.w	lr, lr, #4
	cpsie i  /* enable interrupt */
 8000802:	b662      	cpsie	i
	bx    lr
 8000804:	4770      	bx	lr
 8000806:	0000      	.short	0x0000
	ldr   r1, =SCB_ICSR_REG
 8000808:	e000ed04 	.word	0xe000ed04
	ldr   r1, =is_first_switch		//将is_first_switch值给R1
 800080c:	20000000 	.word	0x20000000

08000810 <_init>:
 8000810:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8000812:	bf00      	nop
 8000814:	bcf8      	pop	{r3, r4, r5, r6, r7}
 8000816:	bc08      	pop	{r3}
 8000818:	469e      	mov	lr, r3
 800081a:	4770      	bx	lr

0800081c <_fini>:
 800081c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800081e:	bf00      	nop
 8000820:	bcf8      	pop	{r3, r4, r5, r6, r7}
 8000822:	bc08      	pop	{r3}
 8000824:	469e      	mov	lr, r3
 8000826:	4770      	bx	lr
