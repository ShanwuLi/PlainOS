# PlainOS

#### 介绍
![logo](logo.svg)
This project is Plain OS, aimed at developing a universal operating system with a plain and minimalist philosophy.

#### 软件架构    

```c++
plain-os                                             顶层目录
   |---- Makefile                                    顶层Makefile（编译入口）
   |
   |---- arch                                        特定架构目录（BSP驱动）
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
   |---- documents                                   OS文档


```

#### 安装教程

1.  从官网下载交叉工具链
2.  修改顶层Makefile中的交叉工具链变量
3.  添加平台相关启动系统代码（BSP）驱动
4.  进入顶层目录make
5.  在apps目录中添加应用层代码，使用xxx_app_register完成app注册

#### 使用说明

1.  xxxx
2.  xxxx
3.  xxxx

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
