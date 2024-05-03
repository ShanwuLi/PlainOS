# 第一章 RTOS软硬件开发环境
本章节旨在搭建一个软硬件平台使得RTOS能够在此平台上运行起来，RTOS开发硬件与软件环境如下。
## 1.1 STM32F103C8T6开发环境
- 硬件平台：https://product.abrobot.club STM32F103C8T6 MINI开发板。
- 软件平台：VSCODE+git+EIDE(VSCODE插件)+FLYMCU(烧录软件)+ATKXCOM(串口软件)。
- 裸机代码：ImplementPlainOS分支Chpter1文件夹中，使用顶层Makefile对文件进行，常用make或make clean指令
## 1.2 重点内容参考资料
- ARM汇编：参考《Arm Compiler Version 6.6 armasm User Guide》或者《The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors》
- 链接脚本：参考网站https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_toc.html
- STM32F103C8T6 BOOT启动方式与外设寄存器讲解：《STM32F101xx, STM32F102xx, STM32F103xx, STM32F105xx and
STM32F107xx advanced Arm®-based 32-bit MCUs Reference manual》参考手册，FLYMCU烧录程序需要了解BOOT启动原理。
- Cortex M3与M4内核说明：《The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors》主要描述内核并讲解了汇编代码
- git使用：《AVR编程权威指南LSW临时版本》
## 1.3 chapter1.3创建裸机工程
- E:\0X01PlainOS\0X00PlainOS\plain-os\chapter1\chapter1.3\arch\arm32\stm32f103c8t6目录下的文件大部分可以通过STM32CubeMX根据具体的芯片型号而生成。
- startup_stm32f10x_md.S汇编文件介绍了芯片的启动过程，各级文件夹下的.MK文件对工程目录进行管理实现文件夹目录子模块分离功能
- 顶层Makefile对整个工程进行管理。