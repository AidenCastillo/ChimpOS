#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t row;
    size_t column;
} cursor_position_t;

// Function declarations
void terminal_initialize(void);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_get_position(cursor_position_t* pos);
void terminal_save_position(void);
void terminal_clear(void);
cursor_position_t terminal_get_prompt_position(void);

#endif // TERMINAL_H
