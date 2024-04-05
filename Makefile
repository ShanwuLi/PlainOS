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
#////////////////////////////// Do not to modify following code //////////////////////////////////#
# get all arguments
ALL_ARGS = $(MAKECMDGOALS)
# get the first argument
FIRST_ARG = $(word 1, $(ALL_ARGS))

TOPDIR   := .
OUTDIR   := out
INC      := -I$(TOPDIR)/include
LIBDIR   :=
LIBS     :=
TARGET   := $(OUTDIR)/plain
DEFINE   := #-DNDEBUG

CXX_SRCS :=
C_SRCS   :=
ASM_SRCS :=
LINK_SCRIPT :=
RM := rm -rf
CPY := cp -a

#-------------------------------------------------------------------------------------------------#
# include sub-makefiles
-include $(TOPDIR)/config.mk
-include $(TOPDIR)/arch/arch.mk
-include $(TOPDIR)/kernel/kernel.mk
-include $(TOPDIR)/drivers/drivers.mk
-include $(TOPDIR)/lib/lib.mk
-include $(TOPDIR)/os_test/os_test.mk
-include $(TOPDIR)/apps/apps.mk

# compiler flags
C_FLAGS   += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)
CXX_FLAGS += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)
ASM_FLAGS += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)
LDFLAGS   += $(MCU) -T$(TARGET).lds $(LIBDIR) $(LIBS) -Wl,-Map=$(TARGET).map,--cref

# list of objects
CXX_OBJS_TMP = $(patsubst %.cpp,%.o,$(CXX_SRCS))
CXX_OBJS := $(addprefix $(OUTDIR)/,$(CXX_OBJS_TMP))

C_OBJS_TMP = $(patsubst %.c,%.o,$(C_SRCS))
C_OBJS := $(addprefix $(OUTDIR)/,$(C_OBJS_TMP))

ASM_OBJS_TMP += $(patsubst %.S,%.o,$(ASM_SRCS))
ASM_OBJS := $(addprefix $(OUTDIR)/,$(ASM_OBJS_TMP))

OBJS := $(C_OBJS) $(ASM_OBJS) $(CXX_OBJS)

CXX_DEPS := $(subst .o,.d,$(CXX_OBJS))
C_DEPS := $(subst .o,.d,$(C_OBJS))
ASM_DEPS := $(subst .o,.d,$(ASM_OBJS))
DEPS := $(C_DEPS) $(ASM_DEPS)

TARGET_FILES := $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).lst $(TARGET).map
DIR_GUARD = @mkdir -p $(@D)

# Configuration Target
%_config:
	@echo "writing config.h"
	@echo "writing config.mk"
	@$(CPY) $(TOPDIR)/configs/$(FIRST_ARG)/config.h $(TOPDIR)/include/
	@$(CPY) $(TOPDIR)/configs/$(FIRST_ARG)/config.mk $(TOPDIR)/
	@echo "write done"

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
	@echo "MK:" $(TARGET).lds
	@$(CC) $(INC) -x assembler-with-cpp -E -P $(LINK_SCRIPT) -o $(TARGET).lds
	@echo "LD:" $@
	@$(CC) $(LDFLAGS) $(OBJS) -o $@  $(LIBS)

$(TARGET).hex: $(TARGET).elf
	@echo "MKIMG:" $@
	@$(CP) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	@echo "MKIMG:" $@
	@$(CP) -O binary -S $< $@

$(TARGET).lst: $(TARGET).elf
	@echo "IMGINFO:" $@
	@$(OBJDUMP) -d --source --all-headers --demangle --line-numbers --wide $< > $@

$(TARGET).size: $(TARGET).elf
	@$(SIZE) --common $<

# Other Targets
.PHONY:clean
clean:
	@echo Removing all output files
ifneq (,$(wildcard $(OUTDIR)/*))
	@$(RM) $(OUTDIR)
endif
	@echo Removing done

.PHONY:disclean
disclean:
	@echo Removing all output and configuration files
ifneq (,$(wildcard $(TOPDIR)/config.mk $(TOPDIR)/include/config.h $(wildcard $(OUTDIR)/*)))
	@$(RM) $(OUTDIR)
	@$(RM) $(TOPDIR)/config.mk
	@$(RM) $(TOPDIR)/include/config.h
endif
	@echo Removing done

rebuild:
	@make clean
	@make all

debug_flags:
	@echo "C_OBJS" : $(C_OBJS)
	@echo "C_FLAGS:" $(C_FLAGS)
	@echo "INC:" $(INC)
	@echo "CHIP:" $(CHIP)
