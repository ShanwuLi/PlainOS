#  MIT License
#
# Copyright (c) 2023 PlainOS
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# compiler flags
DEFINE   += -DSTM32F10X_MD -D__CHECK_DEVICE_DEFINES -D__CM3_REV
TEMP_FLAGS += $(DEFINE) -mthumb -Wall --specs=nosys.specs -Wextra -Wwrite-strings -Wformat=2 \
              -Werror=format-nonliteral -Wvla -Wlogical-op -Wshadow -Wformat-signedness \
              -Wformat-overflow=2 -Wformat-truncation -Werror -Wmissing-declarations \
              -fdiagnostics-color=always -ffunction-sections -fdata-sections -Wall \
              -Werror=all -Werror=deprecated-declarations \
              -Wextra -Werror=unused-parameter -Werror=sign-compare  -gdwarf-4 -ggdb -nostartfiles \
              -fstrict-volatile-bitfields -Werror=unused-but-set-variable -fno-jump-tables \
              -fno-tree-switch-conversion -Werror=unused-function -Werror=unused-variable

C_FLAGS  += $(DEFINE) $(TEMP_FLAGS) -xc -Wmissing-prototypes -Werror=old-style-declaration \
                                    -std=gnu17
CXX_FLAGS += $(TEMP_FLAGS) -xc++ -std=c++14
ASM_FLAGS += -x assembler-with-cpp
LDFLAGS   +=  -Wl,--gc-sections,--print-memory-usage -specs=nano.specs
LIBS      += -lc -lm -lnosys

INC += -I$(ARCH_DIR)/arm32/stm32f103c8t6
INC += -I$(ARCH_DIR)/arm32/stm32f103c8t6/cmsis

ASM_SRCS += $(ARCH_DIR)/arm32/stm32f103c8t6/startup_stm32f10x_md.S
ASM_SRCS += $(ARCH_DIR)/arm32/stm32f103c8t6/stm32f103c8t6_port_asm.S
C_SRCS += $(ARCH_DIR)/arm32/stm32f103c8t6/system_stm32f10x.c
C_SRCS += $(ARCH_DIR)/arm32/stm32f103c8t6/stm32f10x_it.c
C_SRCS += $(ARCH_DIR)/arm32/stm32f103c8t6/stm32f103c8t6_port.c
C_SRCS += $(ARCH_DIR)/arm32/stm32f103c8t6/early_setup/early_uart.c

LINK_SCRIPT := $(ARCH_DIR)/arm32/stm32f103c8t6/stm32f103c8t6.ld

-include $(ARCH_DIR)/arm32/stm32f103c8t6/driver/*.mk
