# compiler flags
DEFINE   += -DSTM32F40X_MD -D__CHECK_DEVICE_DEFINES -D__CM4_REV
TEMP_FLAGS += $(DEFINE) -mthumb -Wall --specs=nosys.specs -Wextra -Wwrite-strings -Wformat=2 \
             -Werror=format-nonliteral -Wvla -Wlogical-op -Wshadow -Wformat-signedness \
             -Wformat-overflow=2 -Wformat-truncation -Werror -Wmissing-declarations \
             -fdiagnostics-color=always -ffunction-sections -fdata-sections -Wall \
             -Werror=all -Werror=unused-function -Werror=unused-variable -Werror=deprecated-declarations \
             -Wextra -Werror=unused-parameter -Werror=sign-compare  -gdwarf-4 -ggdb -nostartfiles \
             -fstrict-volatile-bitfields -Werror=unused-but-set-variable -fno-jump-tables \
             -fno-tree-switch-conversion
C_FLAGS  += $(TEMP_FLAGS) -xc -Wmissing-prototypes -Werror=old-style-declaration \
                            -std=c11
CXX_FLAGS += $(TEMP_FLAGS) -xc++ -std=c++14
ASM_FLAGS += -x assembler-with-cpp
LDFLAGS   += -Wl,--gc-sections,--print-memory-usage -specs=nano.specs
LIBS      += -lc -lm -lnosys

INC += -I$(TOPDIR)/arch/arm32/gd32f407vet6
INC += -I$(TOPDIR)/arch/arm32/gd32f407vet6/cmsis

ASM_SRCS += $(ARCH_DIR)/arm32/gd32f407vet6/startup_gd32f407vet6.S
C_SRCS += $(ARCH_DIR)/arm32/gd32f407vet6/system_stm32f4xx.c
C_SRCS += $(ARCH_DIR)/arm32/gd32f407vet6/stm32f4xx_it.c
C_SRCS += $(ARCH_DIR)/arm32/gd32f407vet6/early_setup/early_uart.c

LINK_SCRIPT := $(ARCH_DIR)/arm32/gd32f407vet6/gd32f407vet6.ld
