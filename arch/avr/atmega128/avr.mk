# compiler flags
C_FLAGS  += $(DEFINE) -Wall -Wextra -Wwrite-strings -Wformat=2 \
           -Werror=format-nonliteral -Wvla -Wlogical-op -Wshadow \
           -Werror -Wmissing-declarations \
           -ffunction-sections -fdata-sections -Wall \
           -Werror=all -Werror=unused-function -Werror=deprecated-declarations \
           -Wextra -Werror=unused-parameter -Werror=sign-compare -ggdb -nostartfiles \
           -fno-jump-tables
CC_FLAGS  += $(C_FLAGS) -xc -Wmissing-prototypes -Werror=old-style-declaration \
                            -std=c99
CXX_FLAGS += $(C_FLAGS) -xc++ -std=c++11
ASM_FLAGS += -x assembler-with-cpp
LDFLAGS   += -Wl,--gc-sections
LIBS      += -lc -lm