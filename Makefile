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



#********************************Attention!!!*********************************#
#makefile注释后面一定不要留空格
#********************************Attention!!!*********************************#



#=================================================================================================#
# please replace to your toolchain.
CROSS_COMPILE := arm-none-eabi-#avr,:=是覆盖之前的赋值，CROSS_COMPILE为变量名称
CC       := $(CROSS_COMPILE)gcc#$(CROSS_COMPILE)对变量的引用
OBJDUMP  := $(CROSS_COMPILE)objdump
CP       := $(CROSS_COMPILE)objcopy
SIZE     := $(CROSS_COMPILE)size
OPTIMIZE := -O3
DEBUG    := -g #-DNDEBUG to close debug in DEFINE.

ARCH     := arm32
MCU      := -mcpu=cortex-m3#-mmcu=atmega128 
CHIP     := stm32f103c8t6#atmega128

#=================================================================================================#
#////////////////////////////// Modify following code when you need it ///////////////////////////#

TOPDIR   := .
COMDIR   := ./chapter2/chapter2.2#COMDIR为参与工程编译的文件根目录
OUTDIR   := $(TOPDIR)/out#直接将中间输出缓冲文件放在TOPDIR中便于烧录

INC      := -I$(COMDIR)/include#-I选项将$(COMDIR)/include路径告诉编译器
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
# include sub-makefiles，-include为Makefile命令可以忽略不存在的文件警告
-include $(COMDIR)/arch/arch.mk
-include $(COMDIR)/kernel/kernel.mk
-include $(COMDIR)/lib/lib.mk

#////////////////////////////// Modify following code when you need it ///////////////////////////#


# compiler flags
C_FLAGS   += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)#+=为追加等号之后的数值，这里是编译器shell命令
CXX_FLAGS += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)
ASM_FLAGS += $(MCU) $(INC) $(OPTIMIZE) $(DEBUG)
LDFLAGS   += $(MCU) -T$(TARGET).lds $(LIBDIR) $(LIBS) -Wl,-Map=$(TARGET).map,--cref

# list of objects，C++工程源文件
CXX_OBJS_TMP = $(patsubst %.cpp,%.o,$(CXX_SRCS))#patsubst是模式字符串替换函数，将.cpp文件名称替换为.o结尾，搜索CXX_SRCS以空格分开的单词
CXX_OBJS := $(addprefix $(OUTDIR)/,$(CXX_OBJS_TMP))#addprefix添加前缀函数，在目标文件名称前面加入out目录路径

C_OBJS_TMP = $(patsubst %.c,%.o,$(C_SRCS))
C_OBJS := $(addprefix $(OUTDIR)/,$(C_OBJS_TMP))

ASM_OBJS_TMP += $(patsubst %.S,%.o,$(ASM_SRCS))
ASM_OBJS := $(addprefix $(OUTDIR)/,$(ASM_OBJS_TMP))


OBJS := $(C_OBJS) $(ASM_OBJS) $(CXX_OBJS)#汇总工程所需的所有.o文件

CXX_DEPS := $(subst .o,.d,$(CXX_OBJS))#字符串替换函数，将.o后缀替换为.d后缀，Depend
C_DEPS := $(subst .o,.d,$(C_OBJS))
ASM_DEPS := $(subst .o,.d,$(ASM_OBJS))
DEPS := $(C_DEPS) $(ASM_DEPS)

TARGET_FILES := $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).lst $(TARGET).map
DIR_GUARD = @mkdir -p $(@D)#创建目录，$(@D)表示当前目录

# All Target
all: $(TARGET_FILES) $(TARGET).size

# Each subdirectory must supply rules for building sources it contributes

$(OUTDIR)/%.o: %.cpp#$<代表第一个依赖文件，$@表示依赖文件，此处$(@)抓取OUTDIR目录
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

# Other Targets，.PHONY起到弱化同级目录下依赖项文件名冲突作用
.PHONY:clean
clean:
	@echo Removing all output files
ifneq (,$(wildcard $(OUTDIR)/*))
	@$(RM) $(OUTDIR)
endif
	@echo Removing done

rebuild:
	@make clean
	@make all

disp_flags:
	@echo "C_OBJS" : $(C_OBJS)
	@echo "C_FLAGS:" $(C_FLAGS)
	@echo "INC:" $(INC)