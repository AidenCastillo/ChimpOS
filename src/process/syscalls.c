#include "syscalls.h"
#include "terminal.h"

void syscall_handler(registers_t* regs) {
    switch (regs->eax) {
        case SYSCALL_WRITE: // sys_write
            syscall_write((const char*)regs->ebx);
            break;
        // Add more syscalls as needed
        case SYSCALL_EXIT: // sys_exit
            // Handle process exit
            break;
        default:
            regs->eax = -1; // Unknown syscall
    }
}
