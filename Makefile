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
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

# Find all assembly files recursively
ASM_SOURCES := $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*/*.s) $(wildcard $(SRC_DIR)/*/*/*.s)
ASM_OBJECTS := $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

# Kernel output
KERNEL = kernel.bin
ISO = os.iso

# Default target
all: iso

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
$(KERNEL): $(BUILD_DIR)/boot.o $(C_OBJECTS) $(ASM_OBJECTS)
	$(LD) -T linker.ld -o $(KERNEL) $(LDFLAGS) $(BUILD_DIR)/boot.o $(C_OBJECTS) $(ASM_OBJECTS) -lgcc

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
