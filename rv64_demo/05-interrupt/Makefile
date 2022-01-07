#
# File: Makefile
# Copyright (c) 2019, Dornerworks Ltd.
#

PROJ_NAME ?= simple
MAKECMDGOALS = qemu
include BuildEnvironment.mk
include FreeRTOS.mk
-include $(wildcard $(OUTDIR)/*.d)
# Platform spcific settings for this application _____________________________________________

ARCH = RISC-V
ARCH_PORTABLE_INC = $(FREERTOS_SOURCE_DIR)/portable/GCC/$(ARCH)/
ARCH_PORTABLE_SRC = $(FREERTOS_SOURCE_DIR)/portable/GCC/$(ARCH)/port.c
ARCH_PORTABLE_ASM = $(FREERTOS_SOURCE_DIR)/portable/GCC/$(ARCH)/portASM.S

SDK_SRC := \
	$(SDK_DIR)/libwrap/sys/_exit.c \
	$(SDK_DIR)/libwrap/sys/close.c \
	$(SDK_DIR)/libwrap/sys/execve.c \
	$(SDK_DIR)/libwrap/sys/fork.c \
	$(SDK_DIR)/libwrap/sys/fstat.c \
	$(SDK_DIR)/libwrap/sys/getpid.c \
	$(SDK_DIR)/libwrap/sys/isatty.c \
	$(SDK_DIR)/libwrap/sys/kill.c \
	$(SDK_DIR)/libwrap/sys/link.c \
	$(SDK_DIR)/libwrap/sys/lseek.c \
	$(SDK_DIR)/libwrap/sys/openat.c \
	$(SDK_DIR)/libwrap/sys/puts.c \
	$(SDK_DIR)/libwrap/sys/read.c \
	$(SDK_DIR)/libwrap/sys/sbrk.c \
	$(SDK_DIR)/libwrap/sys/stat.c \
	$(SDK_DIR)/libwrap/sys/times.c \
	$(SDK_DIR)/libwrap/sys/unlink.c \
	$(SDK_DIR)/libwrap/sys/wait.c \
	$(SDK_DIR)/libwrap/sys/write.c \
	$(SDK_DIR)/libwrap/stdlib/malloc.c \
	$(SDK_DIR)/libwrap/misc/write_hex.c \
	$(SDK_DIR)/plic.c \
	$(SDK_DIR)/uart.c \
	$(SDK_DIR)/printf.c \
	$(SDK_DIR)/timer.c \
        $(SDK_DIR)/trap.c

	#$(SDK_DIR)/init.c \

SDK_ASM := \
	$(SDK_DIR)/entry.S \
	$(SDK_DIR)/start.S
SDK_OBJS := $(patsubst %.c,$(FREERTOS_BUILD_DIR)/%.o,$(notdir $(SDK_SRC)))
SDK_OBJS += $(patsubst %.S,$(FREERTOS_BUILD_DIR)/%.o,$(notdir $(SDK_ASM)))
VPATH += \
	$(SDK_DIR) \
	$(SDK_DIR)/libwrap/sys \
	$(SDK_DIR)/libwrap/misc \
	$(SDK_DIR)/libwrap/stdlib

PORT_OBJS := $(patsubst %.c,$(FREERTOS_BUILD_DIR)/%.o,$(notdir $(ARCH_PORTABLE_SRC)))
PORT_OBJS += $(patsubst %.S,$(FREERTOS_BUILD_DIR)/%.o,$(notdir $(ARCH_PORTABLE_ASM)))
FREERTOS_OBJS += $(PORT_OBJS) $(SDK_OBJS)
VPATH += $(FREERTOS_SOURCE_DIR)/portable/GCC/$(ARCH)


CFLAGS += -DportasmHANDLE_INTERRUPT=$(INTERRUPT_HANDLER)  -ffreestanding
ifeq ($(MAKECMDGOALS),qemu)
	CFLAGS += -D__QEMU__
else ifeq ($(MAKECMDGOALS),nemu)
	CFLAGS += -D__NEMU__
else ifeq ($(MAKECMDGOALS),nutshell)
	CFLAGS += -D__NUTSHELL__
endif

# Set up application source, include, and object files for compilation: ______________________
APP_SRC_DIR = .
APP_SRC = \
	$(APP_SRC_DIR)/main.c \
	$(APP_SRC_DIR)/blinky_demo/main_blinky.c \
	$(APP_SRC_DIR)/full_demo/main_full.c

APP_ASM = \
	$(APP_SRC_DIR)/full_demo/RegTest.S

APP_INCLUDES = \
	-I ./ \
	-I $(SDK_DIR)/include \
	-I $(FREERTOS_INC) \
	-I $(ARCH_PORTABLE_INC) \
	-I $(FREERTOS_DIR)/rv64_demo/Common/include

APP_BUILD_DIR = $(BUILD_DIR)/app
APP_OBJS := $(patsubst %.c,$(APP_BUILD_DIR)/%.o,$(notdir $(APP_SRC)))
APP_OBJS += $(patsubst %.S,$(APP_BUILD_DIR)/%.o,$(notdir $(APP_ASM)))
VPATH += \
	$(APP_SRC_DIR)/blinky_demo \
	$(APP_SRC_DIR)/full_demo \
	$(APP_SRC_DIR)

DEMO_COMMON_SRC =  \
	$(FREERTOS_DIR)/rv64_demo/Common/Minimal/EventGroupsDemo.c \
	$(FREERTOS_DIR)/rv64_demo/Common/Minimal/TimerDemo.c \
	$(FREERTOS_DIR)/rv64_demo/Common/Minimal/TaskNotify.c \
	$(FREERTOS_DIR)/rv64_demo/Common/Minimal/GenQTest.c \
	$(FREERTOS_DIR)/rv64_demo/Common/Minimal/blocktim.c \
	$(FREERTOS_DIR)/rv64_demo/Common/Minimal/dynamic.c \
	$(FREERTOS_DIR)/rv64_demo/Common/Minimal/recmutex.c

DEMO_COMMON_OBJS := $(patsubst %.c,$(APP_BUILD_DIR)/%.o,$(notdir $(DEMO_COMMON_SRC)))
VPATH += $(FREERTOS_DIR)/rv64_demo/Common/Minimal

OUT_ELF = $(BUILD_DIR)/FreeRTOS-$(PROJ_NAME).elf

# Need to tell FreeRTOS where to find the FreeRTOSConfig.h __________________________________
FREERTOS_INCLUDES += \
	-I ./ \
	-I riscv64/include\
	-I $(ARCH_PORTABLE_INC) \
	-I $(SDK_DIR)/include \
	-I $(FREERTOS_DIR)/rv64_demo/Common/include \
	-I $(FREERTOS_SOURCE_DIR)/portable/GCC/RISC-V/chip_specific_extensions/RV32I_CLINT_no_extensions

# List of object files to compile for the system:
OUT_OBJS = \
	$(APP_OBJS) \
	$(DEMO_COMMON_OBJS) \
	$(FREERTOS_OBJS)

BUILD_DIRECTORIES = \
	$(APP_BUILD_DIR) \
	$(FREERTOS_BUILD_DIR)

LDFLAGS += -Wl,-Map,"$(BUILD_DIR)/$(PROJ_NAME).map"

.PHONY: debug clean app_compile debug-app frtos_compile print-info out_elf sim-qemu gdb
all: directories $(OUT_OBJS) $(OUT_ELF)
directories: $(BUILD_DIRECTORIES)
app_compile: directories $(APP_OBJS)
frtos_compile: directories $(FREERTOS_OBJS)
out_elf: directories $(OUT_ELF)

# Notes
# Each "Module" needs
#  - Lists of source files
#  - Source Directory(ies)
#  - Include Directory List (with -I prepending each entry)
#  - Build Directory
#  - Object list
#    - OBJS = $(patsubst %.c,$(MODULE_BUILD_DIR)/%.o,$(notdir $(MODULE_SRC)))
#  - Add source direcories to VPATH
# In the master make file
#  - Module build directory must be created
#  - Object Rules can be done with
#    - $(MODULE_BUILD_DIR)/%.o : %.c

# Compile Object Files ____________________________________________________________________
$(APP_BUILD_DIR)/%.o : %.c
	@echo "[APP Objects] : $@ ______________________________________________________________"
	@echo "Building: $<"
	@mkdir -p $(@D)
	$(GCC) $(CFLAGS) $(APP_INCLUDES) -o $@ -c $<
	@echo "Finished Building: $<"

$(APP_BUILD_DIR)/%.o : %.S
	@echo "[APP Objects] : $@ ______________________________________________________________"
	@echo "Building: $<"
	@mkdir -p $(@D)
	$(GCC) $(ASMFLAGS) $(APP_INCLUDES) $(FREERTOS_INCLUDES) -o $@ -c $<
	@echo "Finished Building: $<"

$(FREERTOS_BUILD_DIR)/%.o : %.c
	@echo "[FreeRTOS Objects] : $@ __________________________________________________________"
	@echo "Building: $<"
	@mkdir -p $(@D)
	$(GCC) $(CFLAGS) $(FREERTOS_INCLUDES) -o $@ -c $<
	@echo "Finished Building: $<"

$(FREERTOS_BUILD_DIR)/%.o : %.S
	@echo "[FreeRTOS Objects] : $@ __________________________________________________________"
	@echo "Building: $<"
	@mkdir -p $(@D)
	$(GCC) $(ASMFLAGS) $(FREERTOS_INCLUDES) -o $@ -c $<
	@echo "Finished Building: $<"

# Generate ELF ___________________________________________________________________________
$(OUT_ELF): $(OUT_OBJS)
	@echo '___________________________________________________________________________________'
	@echo 'Building target: $@'
	@echo '_______________________________'
	@mkdir -p $(@D)
	$(GCC) $(LDFLAGS) $(LDOPTS) -o $@ $(OUT_OBJS) $(LIB_FLAGS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(OBJDUMP) -d $@ > $@.txt
	$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $@ $@.bin

.PHONY:  build_lib

$(BUILD_DIRECTORIES):
	mkdir -p $@

clean:
	rm -rv $(BUILD_DIR)

ifdef USE_RV64
BITS = 64
else
BITS = 32
endif

qemu: $(OUT_ELF)
	qemu-system-riscv$(BITS) -nographic -kernel $^  -machine virt

nemu: $(OUT_ELF)
	$(MAKE) -C $(NEMU_HOME) ISA=riscv$(BITS) run ARGS="-b $(abspath $<.bin)"

nutshell: $(OUT_ELF)
	$(MAKE) -C $(NUTSHELL_HOME) emu IMAGE="$(abspath $<.bin)"

gdb: $(OUT_ELF)
	$(GDB) -iex "set mem inaccessible-by-default off" -iex "set arch riscv:rv$(BITS)" -iex "set riscv use_compressed_breakpoint off"
