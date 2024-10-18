include ../../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# include py core make definitions
include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk

ifeq ($(CROSS_GCC_INSTALL),)
    $(warning provide cross-compiler gcc binary or hardcode within makefile here. CROSS_GCC_INSTALL=<path>)
    CROSS_COMPILE ?= riscv32-unknown-elf-
endif
ifeq ($(CROSS_LLVM_INSTALL),)
    $(warning provide cross-compiler gcc binary or hardcode within makefile here. CROSS_LLVM_INSTALL=<path>)
    CROSS_COMPILE ?= riscv32-unknown-elf-
endif

CROSS_COMPILE ?= $(CROSS_LLVM_INSTALL)/bin/
ifeq ($(UF2_UTILS),)
  $(warning provide path to UF2 utilities or hardcode within makefile here. UF2_UTILS=<path>)
endif

#Screen scrape GCC version
CROSS_GCC_VERSION:=$(shell $(CROSS_GCC_INSTALL)/bin/riscv32-unknown-elf-gcc --version | awk '{if (NR == 1) { print $$3}}')


INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

#####################
#####################
#####################
INC += -I$(HOME)/work/cap-vm/cheriot/riscv-experiment/sonata-rv32e/src/include

CHERIOT_RTOS_INCLUDE_PATH ?= $(HOME)/work/cap-vm/cheriot/cheriot-rtos/sdk/include
CHERIOT_SDK_INCLUDES = $(foreach dir, c++-config libc++ platform/generic-riscv, -I$(CHERIOT_RTOS_INCLUDE_PATH)/$(dir))
INC += $(CHERIOT_SDK_INCLUDES)

###################
#####################

DFU = $(TOP)/tools/dfu.py
PYDFU = $(TOP)/tools/pydfu.py
#CFLAGS_RV32E = -march=rv32em_zicsr_zifencei -mabi=ilp32e -Wdouble-promotion -Wfloat-conversion
CFLAGS_RV32E = -march=rv32em_zicsr_zifencei -mabi=ilp32e -Wfloat-conversion -Wno-unknown-warning-option
CFLAGS += $(INC) -Wall -Werror -std=c99 -nostdlib $(CFLAGS_RV32E) $(COPT)



CSUPEROPT = -Os # save some code space

# Tune for Debugging or Optimization
CFLAGS += -g  # always include debug info in the ELF
ifeq ($(DEBUG), 1)
CFLAGS += -O0
else
CFLAGS += -Os -DNDEBUG
CFLAGS += -fdata-sections -ffunction-sections
endif

# Flags for optional C++ source code
CXXFLAGS += $(filter-out -std=c99,$(CFLAGS)) -std=c++20

LIBS = -L$(CROSS_GCC_INSTALL)/lib/gcc/riscv32-unknown-elf/$(CROSS_GCC_VERSION) -lgcc

SRC_S = crt_mpy.S

SRC_C = main.c                          \
        shared/runtime/pyexec.c         \
        shared/readline/readline.c      \
        shared/libc/printf.c            \
        shared/runtime/stdout_helpers.c \
        $(BUILD)/_frozen_mpy.c          \

SRC_CXX = uart_core.cpp 
ifeq ($(QEMU_RV32),1)
  SRC_C += ns16550a.c
  CFLAGS += -DQEMU_DEBUG
  LDFLAGS += -nostdlib -T rv32e-upy.ld -Map=$@.map --cref --gc-sections
else
  LDFLAGS += -nostdlib -T sonatarv32e.ld -Map=$@.map --cref --gc-sections
#  SRC_CXX += led.cpp
endif

SRC_C += shared/libc/string0.c
SRC_C += $(SRC_LIB_LIBM_C) $(SRC_LIB_LIBM_SQRT_SW_C)
SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c


OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.S=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

all: $(BUILD)/firmware.elf.uf2

$(BUILD)/_frozen_mpy.c: $(TOP)/tests/frozen/frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O binary $^ $@

$(BUILD)/firmware.elf.uf2: $(BUILD)/firmware.elf
	$(ECHO) "Create $@"
	$(Q)$(UF2_UTILS)/utils/uf2conv.py $< -f 0x6CE29E60 -b 0x00000000 -co $@

# Run emulation build on a POSIX system with suitable terminal settings
run:
	@saved_=`stty -g`; \
	  stty raw opost -echo; \
	  $(BUILD)/firmware.elf; \
	  echo "Exit status: $$?"; \
	  stty $$saved_

test: $(BUILD)/firmware.elf
	$(Q)/usr/bin/printf "print('hello world!', list(x+1 for x in range(10)), end='eol\\\\n')\\r\\n\\004" | $(BUILD)/firmware.elf | tail -n2 | grep "^hello world! \\[1, 2, 3, 4, 5, 6, 7, 8, 9, 10\\]eol"

include $(TOP)/py/mkrules.mk
