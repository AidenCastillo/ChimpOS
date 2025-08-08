# ChimpOS

# DEBUG
To execute debug only functions and have automated tests ran at the beginning of boot, add `DEBUG=1` to your make command.
`make run-ramdisk DEBUG=1`

# QEMU and GDB
During one of the custom make bootups, you have the option to boot with QEMU and use the GDB features associated with it.
To run GDB, set `GDB=1` in your make command (ex. `make run-ramdisk GDB=1`). Then in a seperate terminal, run `gdb` and type `target remote localhost:1234`
