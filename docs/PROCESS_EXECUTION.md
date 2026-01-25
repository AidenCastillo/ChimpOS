# Process Execution Guide

## Overview
OS now supports simple single-threaded program execution. Programs can be loaded from the filesystem and executed using the `exec` command.

## How It Works

1. **exec_binary()** - Takes a memory buffer containing executable code and runs it
2. **cmd_exec()** - Shell command that loads a file and passes it to exec_binary()
3. Programs return an exit code that's displayed after execution

## Creating Executable Programs

### Option 1: Simple Function Approach (Current Implementation)

1. Compile your program as a relocatable object file
2. Extract the binary code
3. Store it in your filesystem
4. Execute it with the `exec` command

### Example: Compiling helloWorld.c

```bash
# Compile to object file
i386-elf-gcc -c -ffreestanding -O2 -Iinclude src/drivers/shell/helloWorld.c -o helloWorld.o

# Extract the .text section (executable code)
i386-elf-objcopy -O binary -j .text helloWorld.o helloWorld.bin

# The .bin file can now be loaded into your filesystem
```

Can also run `compile_program.sh` script to automate this.

### Adding Programs to Your Filesystem

You'll need to modify your filesystem initialization to include programs. In your filesystem code, add:

```c
// Load program binary
file_t* hello_prog = fs_open("helloworld", O_CREAT | O_WRONLY);
fs_write(hello_prog, helloworld_binary_data, sizeof(helloworld_binary_data));
fs_close(hello_prog);
```

## Usage in Shell

```
user@host:/$ exec helloworld
Executing helloworld...
Process exited with code: 55
```

## Current Limitations

1. **Single-threaded**: Programs run in the kernel's context
2. **No memory protection**: Programs have full system access
3. **No arguments**: Can't pass command-line arguments yet
4. **Simple format**: Only raw binary code, no ELF parsing
5. **Fixed buffer**: Programs limited to 4KB
