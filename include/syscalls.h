#ifndef SYSCALLS_H
#define SYSCALLS_H

#define SYSCALL_WRITE 1
#define SYSCALL_EXIT 2

#include <stdint.h>

typedef struct {
    uint32_t ds;                  // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

static inline int syscall_write(const char* str) {
    int ret;
    __asm__ __volatile__ (
        "int $0x80"
        : "=a" (ret)
        : "a" (SYSCALL_WRITE), "b" (str)
    );
    return ret;
}

void syscall_handler(registers_t* regs);

#endif
