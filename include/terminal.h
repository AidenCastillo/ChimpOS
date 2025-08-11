#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define HISTORY_LINES 100  // Number of lines to keep in history
// VGA memory address
#define VGA_MEMORY  0xB8000 

// Special characters
#define ESCAPE_CHAR '\033'
#define BACKSPACE_CHAR '\b'
#define NEWLINE '\n'
#define CSI_START '['
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

void terminal_initialize_history(void);
void terminal_get_history(size_t index, char** line);
void terminal_scroll_up(void);
void terminal_scroll_down(void);
void terminal_page_up(void);
void terminal_page_down(void);
int terminal_get_scroll_position(void);

#endif // TERMINAL_H
