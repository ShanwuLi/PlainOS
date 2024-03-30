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

-include $(TOPDIR)/config.mk
OSTEST_DIR := $(TOPDIR)/os_test

ifeq ($(PL_MK_CFG_OS_TEST), y)

# mempool test
ifeq ($(PL_MK_CFG_OS_MEMPOOL_TEST), y)
C_SRCS += $(OSTEST_DIR)/mempool_test.c
endif

# task test
ifeq ($(PL_MK_CFG_OS_TASK_TEST), y)
C_SRCS += $(OSTEST_DIR)/task_test.c
endif

# soft timer test
ifeq ($(PL_MK_CFG_OS_SOFTTIMER_TEST), y)
C_SRCS += $(OSTEST_DIR)/stimer_test.c
endif

# datafifo test
ifeq ($(PL_MK_CFG_OS_KFIFO_TEST), y)
C_SRCS += $(OSTEST_DIR)/kfifo_test.c
endif

endif
