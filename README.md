# PlainOS

## 项目介绍
![logo](logo.png)
Plain含义是简单的意思，该项目旨在开发一个简单而又轻量化的实时操作系统（Real-time operating system, RTOS），因而取名为![logo](logo.svg)，

## plainos软件架构    

```c++
plainos                                             PlainOS顶层目录
   |---- Makefile                                    PlainOS顶层Makefile（编译入口）
   |
   |---- arch                                        特定处理器硬件架构目录（BSP驱动）
   |      |
   |      |--- arm32                                 arm32目录
   |      |     |
   |      |     |---stm32f103c8t6                    stm32f103c8t6特性平台小系统启动目录
   |      |     |    |
   |      |     |    |--- arm32.mk                   小系统启动目录层Makefile
   |      |     |    |--- xxxx.c                     小系统启动目录层源文件
   |      |     |
   |      |     |---stm32f407vet6                    stm32f407vet6特性平台小系统启动目录
   |      |          |--- arm.mk                     小系统启动目录层Makefile
   |      |          |--- xxxx.c                     小系统启动目录层源文件
   |      |
   |      |--- avr                                   avr目录
   |      |     |
   |      |     |---atmega128a                       atmega128a特性平台小系统启动目录
   |      |     |    |--- avr.mk                     小系统启动目录层Makefile
   |      |     |    |--- xxx.c                      小系统启动目录层源文件
   |      |     |
   |      |     |---atmega2560                       atmega2560特性平台小系统启动目录
   |      |     |    |--- avr.mk                     小系统启动目录层Makefile
   |      |     |    |--- xxx.c                      小系统启动目录层源文件
   |      |
   |      |--- riscv                                 risc-v目录
   |            |
   |            |---ch32vf103                        ch32vf103特性平台小系统启动目录
   |                 |--- rescv.mk                   小系统启动目录层Makefile
   |                 |--- xxx.c                      小系统启动目录层源文件
   |
   |
   |---- kernel                                      OS内核目录
   |      |
   |      |---- kernel.mk                            内核Makefile
   |
   |---- drivers                                     驱动目录（BSP驱动）
   |      |
   |      |---- drivers.mk                           驱动目录顶层Makefile
   |      |
   |      |---- core                                 驱动核心抽象层
   |      |      |
   |      |      |--- core.mk                        驱动核心抽象层Makefile
   |      |      |--- gpio                           gpio驱动核心抽象层
   |      |      |--- xxx
   |      |
   |      |---- platform                             平台驱动适配层
   |             |
   |             |--- platform.mk                    平台驱动相关层Makefile
   |             |--- gpio                           gpio驱动核心抽象层
   |             |--- xxx
   |
   |---- include                                     头文件目录(提供给OS上层的通用接口)
   |      |
   |      |---- drivers                              驱动头文件目录
   |      |---- kernel                               内核头文件目录
   |
   |---- apps                                        app目录，应用程序入口
   |
   |---- out                                         镜像输出及obj相关文件
   |
   |---- os_test                                     OS自测试代码
   |
   |---- documents                                   OS文档，使用drawio绘图


```

## PlainOS使用入门

1.  从各处理器官网安装交叉工具链并将其路径添加到环境变量中以便操作系统终端调用。
2.  /plainos目录下执行make xxx_config指令在/plainos目录下生成配置文件，此处xxx_config为/plainos/configs目录下文件夹名称。
3.  /plainos目录执行make指令在/out目录生成可执行二进制文件，此处/符号代表PlainOS工程根目录。
4.  将/out目录下生成的二进制文件烧录到处理器即可运行PlainOS。
   
      以Windows10操作系统使用STM32F103C8T6开发PlainOS为例，用户可在自己指定目录终端下执行下述操作，Linux操作系统同理。
```shell
   git clone https://gitee.com/lsw-elecm/plain-os.git
   cd  plainos
   make stm32f10x_config
   make 
```