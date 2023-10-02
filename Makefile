#=================================================================================================#
# please install esptool using "pip install esptool" on your platform. 
CROSS_COMPILE := arm-none-eabi-
CC       := $(CROSS_COMPILE)gcc
OBJDUMP  := $(CROSS_COMPILE)objdump
CP       := $(CROSS_COMPILE)objcopy
SIZE     := $(CROSS_COMPILE)size

ARCH     := arm
MCU      := -mcpu=cortex-m3
CHIP     := stm32f103c8t6

TOPDIR   := .
OUTDIR   := out
TARGET   := $(OUTDIR)/main
DEFINE   :=

CXX_SRCS := 
C_SRCS   := main.c
ASM_SRCS := 
LINK_SCRIPT :=

##################################################################################################
INC      :=
LIBDIR   :=
LIBS     := -lc -lm -lnosys

RM := rm -rf

#=================================================================================================#
# list of objects
-include arch/$(ARCH)/$(CHIP)/*.mk

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

C_FLAGS += $(DEFINE) -O2 -mthumb -Wall -g --specs=nosys.specs -Wextra -Wwrite-strings -Wformat=2


        #   -Werror=format-nonliteral -Wvla -Wlogical-op -Wshadow -Wformat-signedness \
        #    -Wformat-overflow=2 -Wformat-truncation -Werror -Wmissing-declarations \
        #    -fdiagnostics-color=always -ffunction-sections -fdata-sections -Wall \
        #    -Werror=all -Werror=unused-function -Werror=unused-variable -Werror=deprecated-declarations \
        #    -Wextra -Werror=unused-parameter -Werror=sign-compare  -gdwarf-4 -ggdb -nostartfiles -Og \
        #    -fstrict-volatile-bitfields -Werror=unused-but-set-variable -fno-jump-tables \
        #    -fno-tree-switch-conversion

CC_FLAGS := $(C_FLAGS) -Wmissing-prototypes -Werror=enum-conversion -Werror=old-style-declaration \
                       -std=c11

CXX_FLAGS := $(C_FLAGS) -std=c++14

LDFLAGS := $(MCU) $(LINK_SCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(TARGET).map,--cref -Wl,--gc-sections,--print-memory-usage

TARGET_FILES := $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).lst $(TARGET).map
DIR_GUARD = @mkdir -p $(@D)

# All Target
all:  $(TARGET_FILES) $(TARGET).size 

# Each subdirectory must supply rules for building sources it contributes

$(OUTDIR)/%.o: %.cpp
	@echo "CCXX:" $<
	$(DIR_GUARD)
	@$(CC) -xc++ $(CXX_FLAGS) $(INC) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@

$(OUTDIR)/%.o: %.c
	@echo "CC:" $<
	$(DIR_GUARD)
	@$(CC) -xc $(C_FLAGS) $(INC) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@

$(OUTDIR)/%.o: %.S
	@echo "ASM:" $<
	$(DIR_GUARD)
	@$(CC) -x assembler-with-cpp $(C_FLAGS) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@ 

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# Tool invocations
$(TARGET).elf: $(OBJS)
	@echo "Making:" $@
	@$(CC) $(LDFLAGS) $(OBJS) -o $@  $(LIBS)

$(TARGET).hex: $(TARGET).elf
	$(CP) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(CP) -O binary -S $< $@	

$(TARGET).lst: $(TARGET).elf
	@echo "Making:" $@
	@$(OBJDUMP) -d --source --all-headers --demangle --line-numbers --wide $< > $@

$(TARGET).size: $(TARGET).elf
	@$(SIZE) --format=sysv --common $<


# Other Targets
.PHONY:clean
clean:
	@echo Removing all output files
	@$(RM) $(TARGET_FILES) $(TARGET).bin
ifneq (,$(wildcard $(OUTDIR)/*))
	@$(RM) $(OUTDIR)/*
endif

rebuild:
	@make clean
	@make all

derege:
	@echo $(LINK_SCRIPT)