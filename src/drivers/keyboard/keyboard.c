#include "terminal.h"
#include "keyboard.h"
#include <stdbool.h>

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

// Special keys
#define KEY_PAGE_UP   0x49  // This is the scancode for Page Up
#define KEY_PAGE_DOWN 0x51  // This is the scancode for Page Down

unsigned char keyboard_data_ready() {
    return inb(0x64) & 1;
}

static char char_to_upper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    } else {
        if (c >= '1' && c <= '9') {
            // Handle shifted number keys
            switch (c) {
                case '1': return '!';
                case '2': return '@';
                case '3': return '#';
                case '4': return '$';
                case '5': return '%';
                case '6': return '^';
                case '7': return '&';
                case '8': return '*';
                case '9': return '(';
            }
        } else if (c == '0') {
            return ')';
        } else {
            // Handle other shifted characters
            switch (c) {
                case '-': return '_';
                case '=': return '+';
                case '[': return '{';
                case ']': return '}';
                case '\\': return '|';
                case ';': return ':';
                case '\'': return '"';
                case ',': return '<';
                case '.': return '>';
                case '/': return '?';
                case '`': return '~';
            }
        }
    }


    return c;
}

void read_line(char* buffer, int max_len) {
    int i = 0;
    bool shift_pressed = false;
    while (i < max_len - 1) {
        if (keyboard_data_ready()) {
            unsigned char scancode = inb(0x60);
            
            // Check for Page Up/Down keys
            if (scancode == KEY_PAGE_UP) {
                terminal_page_up();
                continue;
            } else if (scancode == KEY_PAGE_DOWN) {
                terminal_page_down();
                continue;
            }
            
            if (scancode & 0x80) {
                // Key release
                if (scancode == 0xAA || scancode == 0xB6) {
                    // Left or Right Shift release
                    shift_pressed = false;
                }
            } else if (scancode == 0x2A || scancode == 0x36) {
                // Left or Right Shift press
                shift_pressed = true;
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
                    if (shift_pressed) {
                        c = char_to_upper(c);
                    }
                    buffer[i++] = c;
                    terminal_putchar(c);
                }
            }
        }
    }
    buffer[i] = 0;
}
