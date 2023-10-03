# compiler flags
DEFINE   += -DSTM32F10X_MD -D__CHECK_DEVICE_DEFINES -D__CM3_REV
C_FLAGS  += $(DEFINE) -mthumb -Wall --specs=nosys.specs -Wextra -Wwrite-strings -Wformat=2 \
           -Werror=format-nonliteral -Wvla -Wlogical-op -Wshadow -Wformat-signedness \
           -Wformat-overflow=2 -Wformat-truncation -Werror -Wmissing-declarations \
           -fdiagnostics-color=always -ffunction-sections -fdata-sections -Wall \
           -Werror=all -Werror=unused-function -Werror=unused-variable -Werror=deprecated-declarations \
           -Wextra -Werror=unused-parameter -Werror=sign-compare  -gdwarf-4 -ggdb -nostartfiles \
           -fstrict-volatile-bitfields -Werror=unused-but-set-variable -fno-jump-tables \
           -fno-tree-switch-conversion
CC_FLAGS  += $(C_FLAGS) -xc -Wmissing-prototypes -Werror=old-style-declaration \
                            -std=c11
CXX_FLAGS += $(C_FLAGS) -xc++ -std=c++14
ASM_FLAGS += -x assembler-with-cpp
LDFLAGS   += -Wl,--gc-sections,--print-memory-usage -specs=nano.specs
LIBS      += -lc -lm -lnosys

INC += -I$(TOPDIR)/arch/arm/stm32f103c8t6
INC += -I$(TOPDIR)/arch/arm/stm32f103c8t6/cmsis

ASM_SRCS += $(TOPDIR)/arch/arm/stm32f103c8t6/startup_stm32f10x_md.S
C_SRCS += $(TOPDIR)/arch/arm/stm32f103c8t6/system_stm32f10x.c
C_SRCS += $(TOPDIR)/arch/arm/stm32f103c8t6/stm32f10x_it.c