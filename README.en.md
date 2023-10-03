# PlainOS

#### Description
This project is Plain OS, aimed at developing a universal operating system with a plain and minimalist philosophy.

#### Software Architecture
Software architecture description

plain-os                                             顶层目录
   |---- main.c                                      系统入口
   |
   |---- Makefile                                    顶层Makefile
   |
   |---- arch                                        特定架构目录
   |      |
   |      |--- arm                                   arm目录
   |      |     |
   |      |     |---stm32f103c8t6                    stm32f103c8t6特性平台小系统启动目录
   |      |     |    |
   |      |     |    |--- arm.mk                     小系统启动目录层Makefile
   |      |     |    |--- xxxx.c                     小系统启动目录层源文件
   |      |     |
   |      |     |---stm32f407vet6                    stm32f407vet6特性平台小系统启动目录
   |      |          |--- arm.mk                     小系统启动目录层Makefile
   |      |          |--- xxxx.c                     小系统启动目录层源文件
   |      |
   |      |--- avr                                   avr目录
   |      |     |
   |      |     |---atmega128a                       atmega128a特性平台小系统启动目录
   |      |     |    |--- arm.mk                     小系统启动目录层Makefile
   |      |     |    |--- xxx.c                      小系统启动目录层源文件
   |      |     |
   |      |     |---atmega2560                       atmega2560特性平台小系统启动目录
   |      |     |    |--- arm.mk                     小系统启动目录层Makefile
   |      |     |    |--- xxx.c                      小系统启动目录层源文件
   |      |
   |      |--- riscv                                 risc-v目录
   |            |
   |            |---ch32vf103                        ch32vf103特性平台小系统启动目录
   |                 |--- arm.mk                     小系统启动目录层Makefile
   |                 |--- xxx.c                      小系统启动目录层源文件
   |
   |
   |---- kernel                                      OS内核目录
   |      |
   |      |---- kernel.mk                            内核Makefile
   |
   |---- drivers                                     驱动目录
   |      |
   |      |---- driver.mk                            驱动目录顶层Makefile
   |      |
   |      |---- uart                                 串口驱动
   |      |
   |      |----  xxxx
   |
   |---- include                                     头文件目录
   |
   |---- os_test                                    os测试代码


#### Installation

1.  xxxx
2.  xxxx
3.  xxxx

#### Instructions

1.  xxxx
2.  xxxx
3.  xxxx

#### Contribution

1.  Fork the repository
2.  Create Feat_xxx branch
3.  Commit your code
4.  Create Pull Request


#### Gitee Feature

1.  You can use Readme\_XXX.md to support different languages, such as Readme\_en.md, Readme\_zh.md
2.  Gitee blog [blog.gitee.com](https://blog.gitee.com)
3.  Explore open source project [https://gitee.com/explore](https://gitee.com/explore)
4.  The most valuable open source project [GVP](https://gitee.com/gvp)
5.  The manual of Gitee [https://gitee.com/help](https://gitee.com/help)
6.  The most popular members  [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
