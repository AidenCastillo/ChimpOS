# Toolchain
CC = i386-elf-gcc
AS = i386-elf-as
LD = i386-elf-gcc
OBJCOPY = i386-elf-objcopy

GDB ?= 0 # Set to 1 to enable GDB support in QEMU

DEBUG ?= 0 # Set to 1 to enable debug logs

# if debug is enabled, make a c variable for it
ifeq ($(DEBUG),1)
	CVARS += DEBUG
else 
	CVARS := $(filter-out DEBUG,$(CVARS))
endif

# Flags
# Component configuration
FS_TYPE ?= RAMDISK      # Options: RAMDISK, HARDDRIVE
SCHEDULER ?= ROUND_ROBIN # Options: ROUND_ROBIN, PRIORITY
# Add more configurable components

# These become compiler definitions
COMPONENT_FLAGS = -DFS_$(FS_TYPE) -DSCHEDULER_$(SCHEDULER)

# Add to CFLAGS
CFLAGS = -c -g -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude $(COMPONENT_FLAGS) $(foreach var,$(CVARS),-D$(var))
LDFLAGS = -ffreestanding -O2 -nostdlib

# asm flags, intel syntax
ASFLAGS = -msyntax=intel -mnaked-reg -mmnemonic=intel

# Directories
SRC_DIR := src
BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/iso
GRUB_DIR := $(ISO_DIR)/boot/grub

# Find all C files recursively
C_SOURCES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c) $(wildcard $(SRC_DIR)/*/*/*.c)
# Exclude user programs in src/bin/ from kernel compilation
C_SOURCES := $(filter-out $(SRC_DIR)/bin/%,$(C_SOURCES))
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

# Find all assembly files recursively
ASM_SOURCES := $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*/*.s) $(wildcard $(SRC_DIR)/*/*/*.s)
ASM_OBJECTS := $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

# Kernel output
KERNEL = kernel.bin
ISO = os.iso

# Default target
all: iso 

# Find all user programs in src/bin/
BIN_SOURCES := $(wildcard src/bin/*.c)
BIN_BINS := $(patsubst src/bin/%.c,$(BUILD_DIR)/%.bin,$(BIN_SOURCES))
BIN_OBJS := $(patsubst src/bin/%.c,$(BUILD_DIR)/%.bin.o,$(BIN_SOURCES))

# Generic rule to build any .bin from src/bin/*.c
$(BUILD_DIR)/%.bin: src/bin/%.c | $(BUILD_DIR)
	$(CC) -c -ffreestanding -O2 -fno-pic -Iinclude $< -o $(BUILD_DIR)/$*.o
	@if i386-elf-objdump -h $(BUILD_DIR)/$*.o | grep -q ".text.startup"; then \
		$(OBJCOPY) -O binary -j .text.startup $(BUILD_DIR)/$*.o $@; \
	else \
		$(OBJCOPY) -O binary -j .text $(BUILD_DIR)/$*.o $@; \
	fi
	@if [ ! -s $@ ]; then \
		echo "Warning: $@ is empty, trying full binary extraction..."; \
		$(OBJCOPY) -O binary $(BUILD_DIR)/$*.o $@; \
	fi
	@rm -f $(BUILD_DIR)/$*.o

# Generic rule to convert .bin to .bin.o with proper symbol renaming
$(BUILD_DIR)/%.bin.o: $(BUILD_DIR)/%.bin | $(BUILD_DIR)
	$(OBJCOPY) -I binary -O elf32-i386 -B i386 \
		--rename-section .data=.rodata,alloc,load,readonly,data,contents \
		--redefine-sym _binary_build_$*_bin_start=$*_bin_start \
		--redefine-sym _binary_build_$*_bin_end=$*_bin_end \
		--redefine-sym _binary_build_$*_bin_size=$*_bin_size \
		$< $@
# Assemble bootloader (assume boot.s is at project root)
$(BUILD_DIR)/boot.o: boot.s | $(BUILD_DIR)
	$(AS) boot.s -o $@

# Compile all C files into build directory, preserving structure
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# Compile assembly files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) $(ASFLAGS) $< -o $@

# Link everything
$(KERNEL): $(BUILD_DIR)/boot.o $(C_OBJECTS) $(ASM_OBJECTS) $(BIN_OBJS) $(BIN_BINS)
	$(LD) -T linker.ld -o $(KERNEL) $(LDFLAGS) $(BUILD_DIR)/boot.o $(C_OBJECTS) $(ASM_OBJECTS) $(BIN_OBJS) -lgcc

# Create ISO with GRUB
iso: $(KERNEL)
	mkdir -p $(GRUB_DIR)
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(GRUB_DIR)/grub.cfg
	echo "set timeout=0" > $(GRUB_DIR)/grub.cfg
	echo "set default=0" >> $(GRUB_DIR)/grub.cfg
	echo "menuentry 'My OS' {" >> $(GRUB_DIR)/grub.cfg
	echo "multiboot /boot/kernel.bin fs=ramdisk" >> $(GRUB_DIR)/grub.cfg
	echo "}" >> $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Build with specific components
ramdisk-os: export FS_TYPE=RAMDISK
ramdisk-os: all

harddrive-os: export FS_TYPE=HARDDRIVE
harddrive-os: all

# Run in QEMU
$(info GDB value: [$(GDB)])
ifeq ($(GDB),1) # If GDB is set, wait for debugger
	QEMU_FLAGS := -s -S
else
	QEMU_FLAGS :=
endif

# DEBUG
$(info DEBUG value: [$(DEBUG)])

run: iso
	qemu-system-i386 $(QEMU_FLAGS) -cdrom $(ISO)
run-ramdisk: ramdisk-os
	qemu-system-i386 $(QEMU_FLAGS) -cdrom $(ISO)
run-harddrive: harddrive-os
	qemu-system-i386 $(QEMU_FLAGS) -cdrom $(ISO)
# Clean everything
clean:
	rm -rf $(BUILD_DIR) $(KERNEL) $(ISO) isodir

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# OBJS variable definition removed as we're using specific variables for C and ASM objects
