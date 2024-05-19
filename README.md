# 1 项目介绍
本工程为PlainOS分支，旨在一步步实现一个RTOS应有的基础功能。

# 2 工程文件夹介绍
- chapter文件夹为章节笔记配套的实验代码，方便用户直接根据章节内容讲解进行RTOS实验。
- Makefile为管理工程编译的顶层文件，用户可以修改Makefile文件中的COMDIR编译路径即可选择第几章的内容参与编译，从而选择进行第几章的实验内容。
    ```Makefile
    TOPDIR   := .
    COMDIR   := ./chapter1/chapter1.3#COMDIR为参与工程编译的文件根目录
    OUTDIR   := $(TOPDIR)/out#直接将中间输出缓冲文件放在TOPDIR中便于烧录
    ```
- out目录存放编译器输出的二进制烧录等文件，将该目录至于工程根目录便于控制烧录路径不变。
# 3 工程计划实现的功能
## 3.1第一章 RTOS软硬件平台介绍
   
   * 本章节旨在搭建一个软硬件平台使得RTOS能够在此平台上运行起来，实验硬件平台采用https://product.abrobot.club的STM32F103C8T6 MINI最小核心系统板，板载NRST为低电平复位电路，BOOT0控制STM32F103C8T6主控启动方式，High Speed External和Low Speed External分别为8M和32.768KHz，Serial Wire Debug接口为单片机提供调试接口。

   * 开发软件为VSCODE+git+EIDE(VSCODE插件)+FLYMCU(烧录软件)+ATKXCOM(串口软件)。软件开发平台使用VSCode编辑代码，VSCode插件中心软件EIDE提供arm-none-eabi-gcc 源代码编译器，make工具负责管理多文件系统，git可以对代码进行版本控制并在VSCode软件中提供类Linux终端。

## 3.2第二章 任务管理
  - 手动切换2个任务

      手动切换任务需要先创建任务，一个任务不仅包含用户想要的功能代码，还用任务堆栈保存MCU进行上下文切换的寄存器信息，最后还需要任务控制块对任务状态进行记录。第一次创建任务时需要将MCU上下文保存到任务堆栈中，任务调度时MCU执行任务首先从任务堆栈中恢复现场，执行完后将上下文保存到下个任务堆栈中。

  - 手动切换7个任务

      手动实现N个任务和手动实现两个任务之间的区别在于需要修改*pl_callee_get_next_context_sp函数功能与任务栈的创建与分配，本节将函数功能修改为可重入的函数以节省空间，本节在每个任务中输出TASK串口信息。
  
  - 定时切换4个任务
  
      与ARM硬件异常中断有关，与ARM顺序，PendSV和Systick优先级问题。