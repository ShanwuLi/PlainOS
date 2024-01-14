# MIT License
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

#=================================================================================================#
# please install esptool using "pip install esptool" on your platform. 
CROSS_COMPILE := arm-none-eabi-
CC       := $(CROSS_COMPILE)gcc
OBJDUMP  := $(CROSS_COMPILE)objdump
CP       := $(CROSS_COMPILE)objcopy
SIZE     := $(CROSS_COMPILE)size
OPTIMIZE := -O3
DEBUG    := -g #-DNDEBUG to close debug in DEFINE.

ARCH     := arm32
MCU      := -mcpu=cortex-m3#-mmcu=atmega128
CHIP     := stm32f103c8t6#atmega128

#=================================================================================================#
#////////////////////////////// Do not to modify following code //////////////////////////////////#

TOPDIR   := .
OUTDIR   := out
INC      := -I$(TOPDIR)/include 
LIBDIR   :=
LIBS     :=
TARGET   := $(OUTDIR)/$(CHIP)
DEFINE   := #-DNDEBUG

CXX_SRCS :=
C_SRCS   :=
ASM_SRCS :=
LINK_SCRIPT :=
RM := rm -rf

#-------------------------------------------------------------------------------------------------#
# include sub-makefiles
-include $(TOPDIR)/arch/arch.mk
-include $(TOPDIR)/kernel/kernel.mk
-include $(TOPDIR)/drivers/drivers.mk
-include $(TOPDIR)/apps/apps.mk

# compiler flags
C_FLAGS   += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)
CXX_FLAGS += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)
ASM_FLAGS += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)
LDFLAGS   += $(MCU) -T$(TARGET).lds $(LIBDIR) $(LIBS) -Wl,-Map=$(TARGET).map,--cref

# list of objects
CXX_OBJS_TMP = $(patsubst %.cpp,%.o,$(CXX_SRCS))
CXX_OBJS := $(addprefix out/,$(CXX_OBJS_TMP))	# replace ".." to "."

C_OBJS_TMP = $(patsubst %.c,%.o,$(C_SRCS))
C_OBJS := $(addprefix out/,$(C_OBJS_TMP))	# replace ".." to "."

ASM_OBJS_TMP += $(patsubst %.S,%.o,$(ASM_SRCS))
ASM_OBJS := $(addprefix out/,$(ASM_OBJS_TMP))	# replace ".." to "."

OBJS := $(C_OBJS) $(ASM_OBJS) $(CXX_OBJS)

CXX_DEPS := $(subst .o,.d,$(CXX_OBJS))
C_DEPS := $(subst .o,.d,$(C_OBJS))
ASM_DEPS := $(subst .o,.d,$(ASM_OBJS))
DEPS := $(C_DEPS) $(ASM_DEPS)

TARGET_FILES := $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).lst $(TARGET).map
DIR_GUARD = @mkdir -p $(@D)

# All Target
all: $(TARGET_FILES) $(TARGET).size

# Each subdirectory must supply rules for building sources it contributes

$(OUTDIR)/%.o: %.cpp
	@echo "CCXX:" $<
	$(DIR_GUARD)
	@$(CC) $(CXX_FLAGS) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@

$(OUTDIR)/%.o: %.c
	@echo "CC:" $<
	$(DIR_GUARD)
	@$(CC) $(C_FLAGS) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@

$(OUTDIR)/%.o: %.S
	@echo "AS:" $<
	$(DIR_GUARD)
	@$(CC) $(ASM_FLAGS) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# Tool invocations
$(TARGET).elf: $(OBJS)
	@echo "Making:" $(TARGET).lds
	@$(CC) $(INC) -x assembler-with-cpp -E -P $(LINK_SCRIPT) -o $(TARGET).lds
	@echo "Making:" $@
	@$(CC) $(LDFLAGS) $(OBJS) -o $@  $(LIBS)

$(TARGET).hex: $(TARGET).elf
	@echo "Making:" $@
	@$(CP) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	@echo "Making:" $@
	@$(CP) -O binary -S $< $@

$(TARGET).lst: $(TARGET).elf
	@echo "Making:" $@
	@$(OBJDUMP) -d --source --all-headers --demangle --line-numbers --wide $< > $@

$(TARGET).size: $(TARGET).elf
	@$(SIZE) --common $<

# Other Targets
.PHONY:clean
clean:
	@echo Removing all output files
	@$(RM) $(TARGET_FILES) $(TARGET).bin
ifneq (,$(wildcard $(OUTDIR)/*))
	@$(RM) $(OUTDIR)
endif

rebuild:
	@make clean
	@make all

disp_flags:
	@echo "C_OBJS" : $(C_OBJS)
	@echo "C_FLAGS:" $(C_FLAGS)
	@echo "INC:" $(INC)