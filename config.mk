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

# please replace the toolchain to your own.
CROSS_COMPILE := arm-none-eabi-#avr
CC       := $(CROSS_COMPILE)gcc
OBJDUMP  := $(CROSS_COMPILE)objdump
CP       := $(CROSS_COMPILE)objcopy
SIZE     := $(CROSS_COMPILE)size
OPTIMIZE := -O3
DEBUG    := -g#-DNDEBUG to close debug in DEFINE.

ARCH     := arm32
MCU      := -mcpu=cortex-m3#-mmcu=atmega2560
CHIP     := stm32f103c8t6#atmega2560

# TEST CONFIGURATIONS
PL_MK_CFG_OS_TEST = y
PL_MK_CFG_OS_MEMPOOL_TEST = y
PL_MK_CFG_OS_TASK_TEST = y
PL_MK_CFG_OS_SOFTTIMER_TEST = y
PL_MK_CFG_OS_KFIFO_TEST = y
PL_MK_CFG_OS_WORKQUEUE_TEST = y
