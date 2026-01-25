#!/bin/bash
# Script to compile user programs for ChimpOS

if [ $# -eq 0 ]; then
    echo "Usage: $0 <source_file.c>"
    echo "Example: $0 helloworld.c"
    exit 1
fi

SOURCE=$1
BASENAME=$(basename "$SOURCE" .c)
OUTPUT="${BASENAME}.bin"

echo "Compiling $SOURCE for ChimpOS..."

# Compile to object file
i386-elf-gcc -c -ffreestanding -O2 -fno-pic -Iinclude "$SOURCE" -o "${BASENAME}.o"

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Extract binary code - try .text.startup first (for optimized code), then .text
if i386-elf-objdump -h "${BASENAME}.o" | grep -q ".text.startup"; then
    echo "Extracting .text.startup section..."
    i386-elf-objcopy -O binary -j .text.startup "${BASENAME}.o" "$OUTPUT"
else
    echo "Extracting .text section..."
    i386-elf-objcopy -O binary -j .text "${BASENAME}.o" "$OUTPUT"
fi

if [ $? -ne 0 ]; then
    echo "Binary extraction failed!"
    exit 1
fi

# Check if the binary is empty
if [ ! -s "$OUTPUT" ]; then
    echo "Warning: Generated binary is empty!"
    echo "Trying to extract all executable sections..."
    i386-elf-objcopy -O binary "${BASENAME}.o" "$OUTPUT"
fi

echo "Successfully created $OUTPUT"
echo "Binary size: $(stat -c%s "$OUTPUT") bytes"
echo ""
echo "To use in ChimpOS:"
echo "1. Add to your filesystem initialization code"
echo "2. Run 'exec $BASENAME' from the shell"

# Clean up
rm "${BASENAME}.o"
