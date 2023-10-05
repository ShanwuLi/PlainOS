ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           ARM
  Version:                           0x1
  Entry point address:               0x800052d
  Start of program headers:          52 (bytes into file)
  Start of section headers:          153848 (bytes into file)
  Flags:                             0x5000200, Version5 EABI, soft-float ABI
  Size of this header:               52 (bytes)
  Size of program headers:           32 (bytes)
  Number of program headers:         2
  Size of section headers:           40 (bytes)
  Number of section headers:         23
  Section header string table index: 22

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .isr_vector       PROGBITS        08000000 010000 00010c 00   A  0   0  1
  [ 2] .text             PROGBITS        0800010c 01010c 000508 00  AX  0   0  4
  [ 3] .ARM              ARM_EXIDX       08000614 010614 000008 00  AL  2   0  4
  [ 4] .ARM.attributes   ARM_ATTRIBUTES  0800061c 02008c 000029 00      0   0  1
  [ 5] .initcall.init    PROGBITS        0800061c 01061c 000018 00  WA  0   0  4
  [ 6] .init_array       INIT_ARRAY      08000634 010634 000004 04  WA  0   0  4
  [ 7] .fini_array       FINI_ARRAY      08000638 010638 000004 04  WA  0   0  4
  [ 8] .data             PROGBITS        20000000 020000 00008c 00  WA  0   0  4
  [ 9] .bss              NOBITS          2000008c 02008c 00001c 00  WA  0   0  4
  [10] ._user_heap_stack NOBITS          200000a8 02008c 000100 00  WA  0   0  1
  [11] .debug_frame      PROGBITS        00000000 0200b8 0002c4 00      0   0  4
  [12] .debug_info       PROGBITS        00000000 02037c 0016f4 00      0   0  1
  [13] .debug_abbrev     PROGBITS        00000000 021a70 000780 00      0   0  1
  [14] .debug_loc        PROGBITS        00000000 0221f0 000271 00      0   0  1
  [15] .debug_aranges    PROGBITS        00000000 022468 000150 00      0   0  8
  [16] .debug_ranges     PROGBITS        00000000 0225b8 0001d0 00      0   0  8
  [17] .debug_line       PROGBITS        00000000 022788 000cf3 00      0   0  1
  [18] .debug_str        PROGBITS        00000000 02347b 000b0a 01  MS  0   0  1
  [19] .comment          PROGBITS        00000000 023f85 000076 01  MS  0   0  1
  [20] .symtab           SYMTAB          00000000 023ffc 000e60 10     21 137  4
  [21] .strtab           STRTAB          00000000 024e5c 0009a6 00      0   0  1
  [22] .shstrtab         STRTAB          00000000 025802 0000f5 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  y (purecode), p (processor specific)

There are no section groups in this file.

Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  LOAD           0x010000 0x08000000 0x08000000 0x0063c 0x0063c RWE 0x10000
  LOAD           0x020000 0x20000000 0x0800063c 0x0008c 0x001a8 RW  0x10000

 Section to Segment mapping:
  Segment Sections...
   00     .isr_vector .text .ARM .initcall.init .init_array .fini_array 
   01     .data .bss ._user_heap_stack 

There is no dynamic section in this file.

There are no relocations in this file.

Unwind section '.ARM' at offset 0x10614 contains 1 entry:

0x800014c <_start>: 0x1 [cantunwind]


Symbol table '.symtab' contains 230 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 08000000     0 SECTION LOCAL  DEFAULT    1 
     2: 0800010c     0 SECTION LOCAL  DEFAULT    2 
     3: 08000614     0 SECTION LOCAL  DEFAULT    3 
     4: 0800061c     0 SECTION LOCAL  DEFAULT    4 
     5: 0800061c     0 SECTION LOCAL  DEFAULT    5 
     6: 08000634     0 SECTION LOCAL  DEFAULT    6 
     7: 08000638     0 SECTION LOCAL  DEFAULT    7 
     8: 20000000     0 SECTION LOCAL  DEFAULT    8 
     9: 2000008c     0 SECTION LOCAL  DEFAULT    9 
    10: 200000a8     0 SECTION LOCAL  DEFAULT   10 
    11: 00000000     0 SECTION LOCAL  DEFAULT   11 
    12: 00000000     0 SECTION LOCAL  DEFAULT   12 
    13: 00000000     0 SECTION LOCAL  DEFAULT   13 
    14: 00000000     0 SECTION LOCAL  DEFAULT   14 
    15: 00000000     0 SECTION LOCAL  DEFAULT   15 
    16: 00000000     0 SECTION LOCAL  DEFAULT   16 
    17: 00000000     0 SECTION LOCAL  DEFAULT   17 
    18: 00000000     0 SECTION LOCAL  DEFAULT   18 
    19: 00000000     0 SECTION LOCAL  DEFAULT   19 
    20: 00000000     0 FILE    LOCAL  DEFAULT  ABS out/./arch/arm/stm32f103c
    21: f108f85f     0 NOTYPE  LOCAL  DEFAULT  ABS BootRAM
    22: 0800052c     0 NOTYPE  LOCAL  DEFAULT    2 $t
    23: 08000538     0 NOTYPE  LOCAL  DEFAULT    2 LoopCopyDataInit
    24: 08000530     0 NOTYPE  LOCAL  DEFAULT    2 CopyDataInit
    25: 0800054c     0 NOTYPE  LOCAL  DEFAULT    2 LoopFillZerobss
    26: 08000546     0 NOTYPE  LOCAL  DEFAULT    2 FillZerobss
    27: 08000574     0 NOTYPE  LOCAL  DEFAULT    2 Infinite_Loop
    28: 08000574     0 NOTYPE  LOCAL  DEFAULT    2 $t
    29: 08000560     0 NOTYPE  LOCAL  DEFAULT    2 $d
    30: 080001c4     0 NOTYPE  LOCAL  DEFAULT    2 $d
    31: 00000000     0 FILE    LOCAL  DEFAULT  ABS c:/users/lisha/.eide/tool
    32: 080005fc     0 NOTYPE  LOCAL  DEFAULT    2 $t
    33: 08000608     0 NOTYPE  LOCAL  DEFAULT    2 $t
    34: 00000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    35: 0800010c     0 NOTYPE  LOCAL  DEFAULT    2 $t
    36: 0800010d     0 FUNC    LOCAL  DEFAULT    2 __do_global_dtors_aux
    37: 08000124     0 NOTYPE  LOCAL  DEFAULT    2 $d
    38: 2000008c     1 NOTYPE  LOCAL  DEFAULT    9 completed.8885
    39: 08000638     0 NOTYPE  LOCAL  DEFAULT    7 $d
    40: 08000638     0 OBJECT  LOCAL  DEFAULT    7 __do_global_dtors_aux_fin
    41: 08000130     0 NOTYPE  LOCAL  DEFAULT    2 $t
    42: 08000131     0 FUNC    LOCAL  DEFAULT    2 frame_dummy
    43: 08000140     0 NOTYPE  LOCAL  DEFAULT    2 $d
    44: 20000090    24 NOTYPE  LOCAL  DEFAULT    9 object.8890
    45: 08000634     0 NOTYPE  LOCAL  DEFAULT    6 $d
    46: 08000634     0 OBJECT  LOCAL  DEFAULT    6 __frame_dummy_init_array_
    47: 2000008c     0 NOTYPE  LOCAL  DEFAULT    9 $d
    48: 00000000     0 FILE    LOCAL  DEFAULT  ABS c:/users/lisha/.eide/tool
    49: 0800014c     0 NOTYPE  LOCAL  DEFAULT    2 $t
    50: 080001a0     0 NOTYPE  LOCAL  DEFAULT    2 $d
    51: 08000614     0 NOTYPE  LOCAL  DEFAULT    3 $d
    52: 00000000     0 FILE    LOCAL  DEFAULT  ABS exit.c
    53: 080001d8     0 NOTYPE  LOCAL  DEFAULT    2 $t
    54: 080001f8     0 NOTYPE  LOCAL  DEFAULT    2 $d
    55: 00000000     0 FILE    LOCAL  DEFAULT  ABS impure.c
    56: 20000000    96 OBJECT  LOCAL  DEFAULT    8 impure_data
    57: 20000000     0 NOTYPE  LOCAL  DEFAULT    8 $d
    58: 08000578     0 NOTYPE  LOCAL  DEFAULT    2 $d
    59: 00000000     0 FILE    LOCAL  DEFAULT  ABS init.c
    60: 08000200     0 NOTYPE  LOCAL  DEFAULT    2 $t
    61: 08000238     0 NOTYPE  LOCAL  DEFAULT    2 $d
    62: 00000000     0 FILE    LOCAL  DEFAULT  ABS memset.c
    63: 08000248     0 NOTYPE  LOCAL  DEFAULT    2 $t
    64: 00000000     0 FILE    LOCAL  DEFAULT  ABS _exit.c
    65: 08000258     0 NOTYPE  LOCAL  DEFAULT    2 $t
    66: 00000000     0 FILE    LOCAL  DEFAULT  ABS main.c
    67: 0800025c     0 NOTYPE  LOCAL  DEFAULT    2 $t
    68: 0800025d    20 FUNC    LOCAL  DEFAULT    2 UART_INIT0
    69: 0800026c     0 NOTYPE  LOCAL  DEFAULT    2 $d
    70: 08000270     0 NOTYPE  LOCAL  DEFAULT    2 $t
    71: 08000271    16 FUNC    LOCAL  DEFAULT    2 UART_INIT1
    72: 0800027c     0 NOTYPE  LOCAL  DEFAULT    2 $d
    73: 08000280     0 NOTYPE  LOCAL  DEFAULT    2 $t
    74: 08000281    16 FUNC    LOCAL  DEFAULT    2 UART_INIT2
    75: 0800028c     0 NOTYPE  LOCAL  DEFAULT    2 $d
    76: 08000290     0 NOTYPE  LOCAL  DEFAULT    2 $t
    77: 08000291    16 FUNC    LOCAL  DEFAULT    2 UART_INIT3
    78: 0800029c     0 NOTYPE  LOCAL  DEFAULT    2 $d
    79: 080002a0     0 NOTYPE  LOCAL  DEFAULT    2 $t
    80: 080002a1    16 FUNC    LOCAL  DEFAULT    2 UART_INIT4
    81: 080002ac     0 NOTYPE  LOCAL  DEFAULT    2 $d
    82: 080002b0     0 NOTYPE  LOCAL  DEFAULT    2 $t
    83: 080002b1    16 FUNC    LOCAL  DEFAULT    2 UART_INIT5
    84: 080002bc     0 NOTYPE  LOCAL  DEFAULT    2 $d
    85: 080002c0     0 NOTYPE  LOCAL  DEFAULT    2 $t
    86: 0800034c     0 NOTYPE  LOCAL  DEFAULT    2 $d
    87: 080001c0     0 NOTYPE  LOCAL  DEFAULT    2 $t
    88: 080001c1     4 FUNC    LOCAL  DEFAULT    2 dhwehdf
    89: 20000060     0 NOTYPE  LOCAL  DEFAULT    8 $d
    90: 20000060    44 OBJECT  LOCAL  DEFAULT    8 initcall_levels
    91: 0800061c     0 NOTYPE  LOCAL  DEFAULT    5 $d
    92: 0800061c     4 OBJECT  LOCAL  DEFAULT    5 __initcall_UART_INIT00
    93: 08000620     0 NOTYPE  LOCAL  DEFAULT    5 $d
    94: 08000620     4 OBJECT  LOCAL  DEFAULT    5 __initcall_UART_INIT10s
    95: 08000624     0 NOTYPE  LOCAL  DEFAULT    5 $d
    96: 08000624     4 OBJECT  LOCAL  DEFAULT    5 __initcall_UART_INIT21
    97: 08000628     0 NOTYPE  LOCAL  DEFAULT    5 $d
    98: 08000628     4 OBJECT  LOCAL  DEFAULT    5 __initcall_UART_INIT31s
    99: 0800062c     0 NOTYPE  LOCAL  DEFAULT    5 $d
   100: 0800062c     4 OBJECT  LOCAL  DEFAULT    5 __initcall_UART_INIT48
   101: 08000630     0 NOTYPE  LOCAL  DEFAULT    5 $d
   102: 08000630     4 OBJECT  LOCAL  DEFAULT    5 __initcall_UART_INIT59
   103: 0800057c     0 NOTYPE  LOCAL  DEFAULT    2 $d
   104: 0800058c     0 NOTYPE  LOCAL  DEFAULT    2 $d
   105: 080005a4     0 NOTYPE  LOCAL  DEFAULT    2 $d
   106: 080005b4     0 NOTYPE  LOCAL  DEFAULT    2 $d
   107: 080005cc     0 NOTYPE  LOCAL  DEFAULT    2 $d
   108: 080005e0     0 NOTYPE  LOCAL  DEFAULT    2 $d
   109: 080005f0     0 NOTYPE  LOCAL  DEFAULT    2 $d
   110: 00000000     0 FILE    LOCAL  DEFAULT  ABS system_stm32f10x.c
   111: 08000360     0 NOTYPE  LOCAL  DEFAULT    2 $t
   112: 08000454     0 NOTYPE  LOCAL  DEFAULT    2 $d
   113: 00000000     0 FILE    LOCAL  DEFAULT  ABS stm32f10x_it.c
   114: 08000464     0 NOTYPE  LOCAL  DEFAULT    2 $t
   115: 08000468     0 NOTYPE  LOCAL  DEFAULT    2 $t
   116: 0800046c     0 NOTYPE  LOCAL  DEFAULT    2 $t
   117: 08000470     0 NOTYPE  LOCAL  DEFAULT    2 $t
   118: 08000474     0 NOTYPE  LOCAL  DEFAULT    2 $t
   119: 08000478     0 NOTYPE  LOCAL  DEFAULT    2 $t
   120: 0800047c     0 NOTYPE  LOCAL  DEFAULT    2 $t
   121: 00000000     0 FILE    LOCAL  DEFAULT  ABS list.c
   122: 00000000     0 FILE    LOCAL  DEFAULT  ABS USART.c
   123: 08000480     0 NOTYPE  LOCAL  DEFAULT    2 $t
   124: 080004f8     0 NOTYPE  LOCAL  DEFAULT    2 $d
   125: 0800050c     0 NOTYPE  LOCAL  DEFAULT    2 $t
   126: 08000528     0 NOTYPE  LOCAL  DEFAULT    2 $d
   127: 00000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
   128: 00000000     0 FILE    LOCAL  DEFAULT  ABS c:/users/lisha/.eide/tool
   129: 08000600     0 NOTYPE  LOCAL  DEFAULT    2 $t
   130: 0800060c     0 NOTYPE  LOCAL  DEFAULT    2 $t
   131: 00000000     0 FILE    LOCAL  DEFAULT  ABS 
   132: 08000638     0 NOTYPE  LOCAL  DEFAULT    6 __init_array_end
   133: 0800061c     0 NOTYPE  LOCAL  DEFAULT    4 __preinit_array_end
   134: 08000634     0 NOTYPE  LOCAL  DEFAULT    6 __init_array_start
   135: 0800061c     0 NOTYPE  LOCAL  DEFAULT    4 __preinit_array_start
   136: 08000000     0 NOTYPE  LOCAL  DEFAULT    1 $d
   137: 08000575     2 FUNC    WEAK   DEFAULT    2 EXTI2_IRQHandler
   138: 0800047d     2 FUNC    GLOBAL DEFAULT    2 DebugMon_Handler
   139: 08000575     2 FUNC    WEAK   DEFAULT    2 TIM1_CC_IRQHandler
   140: 08000469     2 FUNC    GLOBAL DEFAULT    2 HardFault_Handler
   141: 00000100     0 NOTYPE  GLOBAL DEFAULT  ABS _Min_Stack_Size
   142: 08000575     2 FUNC    WEAK   DEFAULT    2 SysTick_Handler
   143: 08000575     2 FUNC    WEAK   DEFAULT    2 PVD_IRQHandler
   144: 0800063c     0 NOTYPE  GLOBAL DEFAULT    7 _sidata
   145: 08000575     2 FUNC    WEAK   DEFAULT    2 PendSV_Handler
   146: 08000465     2 FUNC    GLOBAL DEFAULT    2 NMI_Handler
   147: 0800061c     0 NOTYPE  GLOBAL DEFAULT    3 __exidx_end
   148: 08000575     2 FUNC    WEAK   DEFAULT    2 EXTI3_IRQHandler
   149: 08000614     0 NOTYPE  GLOBAL DEFAULT    2 _etext
   150: 2000008c     0 NOTYPE  GLOBAL DEFAULT    9 _sbss
   151: 08000634     0 NOTYPE  GLOBAL DEFAULT    5 __initcall_end
   152: 08000575     2 FUNC    WEAK   DEFAULT    2 EXTI0_IRQHandler
   153: 08000575     2 FUNC    WEAK   DEFAULT    2 I2C2_EV_IRQHandler
   154: 08000475     2 FUNC    GLOBAL DEFAULT    2 UsageFault_Handler
   155: 08000575     2 FUNC    WEAK   DEFAULT    2 ADC1_2_IRQHandler
   156: 2000008c     0 NOTYPE  GLOBAL DEFAULT    9 __bss_start__
   157: 20000000     0 NOTYPE  GLOBAL DEFAULT    8 _sdata
   158: 08000575     2 FUNC    WEAK   DEFAULT    2 SPI1_IRQHandler
   159: 08000575     2 FUNC    WEAK   DEFAULT    2 TAMPER_IRQHandler
   160: 08000614     0 NOTYPE  GLOBAL DEFAULT    3 __exidx_start
   161: 08000578     4 OBJECT  GLOBAL DEFAULT    2 _global_impure_ptr
   162: 0800061c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall0_start
   163: 08000201    72 FUNC    GLOBAL DEFAULT    2 __libc_init_array
   164: 08000575     2 FUNC    WEAK   DEFAULT    2 DMA1_Channel4_IRQHandler
   165: 0800014d     0 FUNC    GLOBAL DEFAULT    2 _mainCRTStartup
   166: 080005fd     0 FUNC    GLOBAL DEFAULT    2 _init
   167: 0800050d    32 FUNC    GLOBAL DEFAULT    2 USART1_PrintString
   168: 08000575     2 FUNC    WEAK   DEFAULT    2 USART3_IRQHandler
   169: 08000575     2 FUNC    WEAK   DEFAULT    2 RTC_IRQHandler
   170: 200000a8     0 NOTYPE  GLOBAL DEFAULT    9 _ebss
   171: 08000575     2 FUNC    WEAK   DEFAULT    2 DMA1_Channel7_IRQHandler
   172: 0800052d    52 FUNC    WEAK   DEFAULT    2 Reset_Handler
   173: 08000575     2 FUNC    WEAK   DEFAULT    2 CAN1_RX1_IRQHandler
   174: 0800062c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall4_start
   175: 08000575     2 FUNC    WEAK   DEFAULT    2 TIM4_IRQHandler
   176: 08000575     2 FUNC    WEAK   DEFAULT    2 I2C1_EV_IRQHandler
   177: 08000575     2 FUNC    WEAK   DEFAULT    2 DMA1_Channel6_IRQHandler
   178: 200000a8     0 NOTYPE  GLOBAL DEFAULT    9 __bss_end__
   179: 08000575     2 FUNC    WEAK   DEFAULT    2 TIM3_IRQHandler
   180: 08000575     2 FUNC    WEAK   DEFAULT    2 RCC_IRQHandler
   181: 08000481   140 FUNC    GLOBAL DEFAULT    2 USART1_Init
   182: 08000575     2 FUNC    WEAK   DEFAULT    2 TIM1_TRG_COM_IRQHandler
   183: 00000000     0 NOTYPE  GLOBAL DEFAULT  ABS _Min_Heap_Size
   184: 08000575     2 FUNC    WEAK   DEFAULT    2 DMA1_Channel1_IRQHandler
   185: 08000574     2 NOTYPE  GLOBAL DEFAULT    2 Default_Handler
   186: 0800014d     0 FUNC    GLOBAL DEFAULT    2 _start
   187: 08000575     2 FUNC    WEAK   DEFAULT    2 EXTI15_10_IRQHandler
   188: 08000575     2 FUNC    WEAK   DEFAULT    2 EXTI9_5_IRQHandler
   189: 0800062c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall6_start
   190: 08000575     2 FUNC    WEAK   DEFAULT    2 SPI2_IRQHandler
   191: 08000249    16 FUNC    GLOBAL DEFAULT    2 memset
   192: 0800046d     2 FUNC    GLOBAL DEFAULT    2 MemManage_Handler
   193: 080002c1   160 FUNC    GLOBAL DEFAULT    2 main
   194: 08000479     2 FUNC    GLOBAL DEFAULT    2 SVC_Handler
   195: 08000630     0 NOTYPE  GLOBAL DEFAULT    5 __initcall9_start
   196: 08000575     2 FUNC    WEAK   DEFAULT    2 DMA1_Channel5_IRQHandler
   197: 08000575     2 FUNC    WEAK   DEFAULT    2 EXTI4_IRQHandler
   198: 08000575     2 FUNC    WEAK   DEFAULT    2 USB_LP_CAN1_RX0_IRQHandle
   199: 08000361   260 FUNC    GLOBAL DEFAULT    2 SystemInit
   200: 08000609     0 FUNC    GLOBAL DEFAULT    2 _fini
   201: 08000575     2 FUNC    WEAK   DEFAULT    2 USB_HP_CAN1_TX_IRQHandler
   202: 08000575     2 FUNC    WEAK   DEFAULT    2 DMA1_Channel3_IRQHandler
   203: 08000575     2 FUNC    WEAK   DEFAULT    2 TIM1_UP_IRQHandler
   204: 0800062c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall5_start
   205: 08000575     2 FUNC    WEAK   DEFAULT    2 WWDG_IRQHandler
   206: 08000575     2 FUNC    WEAK   DEFAULT    2 TIM2_IRQHandler
   207: 0800062c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall8_start
   208: 0800061c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall_start
   209: 08000575     2 FUNC    WEAK   DEFAULT    2 TIM1_BRK_IRQHandler
   210: 20005000     0 NOTYPE  GLOBAL DEFAULT  ABS _estack
   211: 08000575     2 FUNC    WEAK   DEFAULT    2 EXTI1_IRQHandler
   212: 2000008c     0 NOTYPE  GLOBAL DEFAULT    8 _edata
   213: 08000575     2 FUNC    WEAK   DEFAULT    2 RTCAlarm_IRQHandler
   214: 08000575     2 FUNC    WEAK   DEFAULT    2 USART2_IRQHandler
   215: 08000624     0 NOTYPE  GLOBAL DEFAULT    5 __initcall1_start
   216: 080001d9    40 FUNC    GLOBAL DEFAULT    2 exit
   217: 08000000     0 OBJECT  GLOBAL DEFAULT    1 g_pfnVectors
   218: 0800062c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall7_start
   219: 08000575     2 FUNC    WEAK   DEFAULT    2 I2C2_ER_IRQHandler
   220: 08000575     2 FUNC    WEAK   DEFAULT    2 DMA1_Channel2_IRQHandler
   221: 08000575     2 FUNC    WEAK   DEFAULT    2 CAN1_SCE_IRQHandler
   222: 08000575     2 FUNC    WEAK   DEFAULT    2 FLASH_IRQHandler
   223: 08000259     2 FUNC    GLOBAL DEFAULT    2 _exit
   224: 08000471     2 FUNC    GLOBAL DEFAULT    2 BusFault_Handler
   225: 08000575     2 FUNC    WEAK   DEFAULT    2 USART1_IRQHandler
   226: 08000575     2 FUNC    WEAK   DEFAULT    2 I2C1_ER_IRQHandler
   227: 0800062c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall3_start
   228: 0800062c     0 NOTYPE  GLOBAL DEFAULT    5 __initcall2_start
   229: 08000575     2 FUNC    WEAK   DEFAULT    2 USBWakeUp_IRQHandler

No version information found in this file.
Attribute Section: aeabi
File Attributes
  Tag_CPU_name: "7-M"
  Tag_CPU_arch: v7
  Tag_CPU_arch_profile: Microcontroller
  Tag_THUMB_ISA_use: Thumb-2
  Tag_ABI_PCS_wchar_t: 4
  Tag_ABI_FP_denormal: Needed
  Tag_ABI_FP_exceptions: Needed
  Tag_ABI_FP_number_model: IEEE 754
  Tag_ABI_align_needed: 8-byte
  Tag_ABI_enum_size: small
  Tag_CPU_unaligned_access: v6
