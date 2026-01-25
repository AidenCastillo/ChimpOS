# ChimpOS
Currently only runs with a i386-elf cross compiler. Will get that fixed to run x86-64 and i686 cross compilers.

# Building
Requirements:
- i386-elf-gcc
- i386-elf-ld
- i386-elf-objcopy
- grub-mkrescue
- xorriso
- qemu-system-i386

You can receive the i386-elf toolchain by downloading from Github releases.

To build the OS, simply run `make all` in the root directory. This will compile all source files and create the necessary binaries.


# Running
Either run `qemu-system-i386 <QEMU FLAGS> -cdrom build/os.iso` or use the provided Makefile target.
- To run and build with RAM disk: `make run-ramdisk`
- To run and build with hard disk image: `make run-harddrive`

# Cleaning
To clean the build artifacts, run `make clean`. This will remove all compiled files and binaries from the build directory.


# DEBUG
To execute debug only functions and have automated tests ran at the beginning of boot, add `DEBUG=1` to your make command.
`make run-ramdisk DEBUG=1`

# QEMU and GDB
During one of the custom make bootups, you have the option to boot with QEMU and use the GDB features associated with it.
To run GDB, set `GDB=1` in your make command (ex. `make run-ramdisk GDB=1`). Then in a separate terminal, run `gdb` and type `target remote localhost:1234`


# Additional Documentation
- [Process Execution Guide](docs/PROCESS_EXECUTION.md)
- [Filesystem Guide](docs/FILESYSTEM.md)
