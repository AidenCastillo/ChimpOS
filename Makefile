# Toolchain
CC = i386-elf-gcc
AS = i386-elf-as
LD = i386-elf-gcc
OBJCOPY = i386-elf-objcopy

# Flags
CFLAGS = -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -ffreestanding -O2 -nostdlib

# Directories
SRC_DIR := src
BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/iso
GRUB_DIR := $(ISO_DIR)/boot/grub

# Find all C files recursively
C_SOURCES := $(shell find $(SRC_DIR) -name '*.c')
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

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

# Link everything
$(KERNEL): $(BUILD_DIR)/boot.o $(C_OBJECTS)
	$(LD) -T linker.ld -o $(KERNEL) $(LDFLAGS) $(BUILD_DIR)/boot.o $(C_OBJECTS) -lgcc

# Create ISO with GRUB
iso: $(KERNEL)
	mkdir -p $(GRUB_DIR)
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(GRUB_DIR)/grub.cfg
	echo "set timeout=0" > $(GRUB_DIR)/grub.cfg
	echo "set default=0" >> $(GRUB_DIR)/grub.cfg
	echo "menuentry 'My OS' {" >> $(GRUB_DIR)/grub.cfg
	echo "multiboot /boot/kernel.bin" >> $(GRUB_DIR)/grub.cfg
	echo "}" >> $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Run in QEMU
run: iso
	qemu-system-i386 -cdrom $(ISO)

# Clean everything
clean:
	rm -rf $(BUILD_DIR) $(KERNEL) $(ISO) isodir

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
