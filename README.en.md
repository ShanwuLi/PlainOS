# PlainOS

#### Description
This project is Plain OS, aimed at developing a universal operating system with a plain and minimalist philosophy.

#### Software Architecture
Software architecture description
```c++
plain-os                                             top dirctory
   |---- main.c                                      system entry
   |
   |---- Makefile                                    top Makefile
   |
   |---- arch                                        specifical architecture
   |      |
   |      |--- arm                                   arm dirctory
   |      |     |
   |      |     |---stm32f103c8t6                    stm32f103c8t6 platform feature setup code
   |      |     |    |
   |      |     |    |--- arm.mk                     top Makefile of setup code
   |      |     |    |--- xxxx.c                     source files of setup code
   |      |     |
   |      |     |---stm32f407vet6                    stm32f407vet6 platform feature setup code
   |      |          |--- arm.mk                     top Makefile of setup code
   |      |          |--- xxxx.c                     source files of setup code
   |      |
   |      |--- avr                                   avr dirctory
   |      |     |
   |      |     |---atmega128a                       atmega128a platform feature setup code
   |      |     |    |--- arm.mk                     top Makefile of setup code
   |      |     |    |--- xxx.c                      source files of setup code
   |      |     |
   |      |     |---atmega2560                       atmega2560 platform feature setup code
   |      |     |    |--- arm.mk                     top Makefile of setup code
   |      |     |    |--- xxx.c                      source files of setup code
   |      |
   |      |--- riscv                                 risc-v dirctory
   |            |
   |            |---ch32vf103                        ch32vf103 platform feature setup code
   |                 |--- arm.mk                     top Makefile of setup code
   |                 |--- xxx.c                      source files of setup code
   |
   |  
   |---- kernel                                      OS kernel dirctory
   |      |
   |      |---- kernel.mk                            kernel Makefile
   |
   |---- drivers                                     driver dirctory
   |      |
   |      |---- drivers.mk                           top Makefile of driver code
   |      |
   |      |---- uart                                 uart driver
   |      |
   |      |----  xxxx
   | 
   |---- include                                     head file dirctory
   |
   |---- os_test                                     OS test code
```

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
