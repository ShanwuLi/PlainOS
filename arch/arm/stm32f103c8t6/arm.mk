DEFINE += -DSTM32F10X_MD -D__CHECK_DEVICE_DEFINES -D__CM3_REV
INC    += -I$(TOPDIR)/arch/arm/stm32f103c8t6
INC    += -I$(TOPDIR)/arch/arm/stm32f103c8t6/cmsis

LINK_SCRIPT += -T$(TOPDIR)/arch/arm/stm32f103c8t6/stm32f103c8t6.ld
ASM_SRCS += $(TOPDIR)/arch/arm/stm32f103c8t6/startup_stm32f10x_md.S
C_SRCS += $(TOPDIR)/arch/arm/stm32f103c8t6/system_stm32f10x.c
C_SRCS += $(TOPDIR)/arch/arm/stm32f103c8t6/stm32f10x_it.c
C_SRCS += $(TOPDIR)/arch/arm/stm32f103c8t6/USART.c