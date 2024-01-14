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
TEMP_FLAGS += $(DEFINE) -Wall -Wextra -Wwrite-strings -Wformat=2 \
              -Werror=format-nonliteral -Wvla -Wlogical-op -Wshadow \
              -Werror -Wmissing-declarations \
              -ffunction-sections -fdata-sections -Wall \
              -Werror=all -Werror=unused-function -Werror=deprecated-declarations \
              -Wextra -Werror=unused-parameter -Werror=sign-compare -ggdb -nostartfiles \
              -fno-jump-tables
C_FLAGS   += $(TEMP_FLAGS) -xc -Wmissing-prototypes -Werror=old-style-declaration \
                           -std=c99
CXX_FLAGS += $(TEMP_FLAGS) -xc++ -std=c++99
ASM_FLAGS += -x assembler-with-cpp
LDFLAGS   += -Wl,--gc-sections
LIBS      += -lc -lm

LINK_SCRIPT := $(ARCH_DIR)/avr/atmega128/atmega128.ld