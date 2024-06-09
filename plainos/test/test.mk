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
OSTEST_DIR := $(TOPDIR)/test

ifeq ($(PL_OS_TEST), y)

# mempool test
ifeq ($(PL_OS_TEST_MEMPOOL), y)
C_SRCS += $(OSTEST_DIR)/mempool_test.c
endif

# task test
ifeq ($(PL_OS_TEST_TASK), y)
C_SRCS += $(OSTEST_DIR)/task_test.c
endif

# soft timer test
ifeq ($(PL_OS_TEST_SOFTTIMER), y)
C_SRCS += $(OSTEST_DIR)/stimer_test.c
endif

# datafifo test
ifeq ($(PL_OS_TEST_KFIFO), y)
C_SRCS += $(OSTEST_DIR)/kfifo_test.c
endif

# workqueue test
ifeq ($(PL_OS_TEST_WORKQUEUE), y)
C_SRCS += $(OSTEST_DIR)/workqueue_test.c
endif

endif
