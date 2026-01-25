// Attempt to use terminal_writestring to output execution
// ATTENTION: terminal_writestring will cause os crash
// #include <stddef.h>
#include "syscalls.h"

int main() {
    // Simple test program
    syscall_write("Hello from user program terminalProg!\n");
    return 0;
}
