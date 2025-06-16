#include "terminal.h"

unsigned char inb(unsigned short port)
{
    unsigned char ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', 
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, /* ... rest ... */
};

unsigned char keyboard_data_ready() {
    return inb(0x64) & 1;
}

void read_line(char* buffer, int max_len) {
    int i = 0;
    while (i < max_len - 1) {
        if (keyboard_data_ready()) {
            unsigned char scancode = inb(0x60);
            if (scancode & 0x80) {
                
            } else {
                char c = keyboard_map[scancode];
                if (c == '\n') {
                    buffer[i] = 0;
                    terminal_putchar('\n');
                    break;
                } else if (c == '\b' && i > 0) {
                    i--;
                    terminal_putchar('\b');
                    // Optional: Clear the character on screen
                    terminal_putchar(' ');
                    terminal_putchar('\b');
                } else if (c) {
                    buffer[i++] = c;
                    terminal_putchar(c);
                }
            }
        }
    }
    buffer[i] = 0;
}
