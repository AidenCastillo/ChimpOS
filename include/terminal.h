#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define HISTORY_LINES 100  // Number of lines to keep in history
// VGA memory address
#define VGA_MEMORY  0xB8000 

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

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
void terminal_set_color(uint8_t color);
void terminal_clear(void);
cursor_position_t terminal_get_prompt_position(void);

// Color management functions
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
void terminal_set_fg_color(enum vga_color fg);
void terminal_set_bg_color(enum vga_color bg);
void terminal_write_colored(const char* data, size_t size, enum vga_color fg, enum vga_color bg);
void terminal_writestring_colored(const char* data, enum vga_color fg, enum vga_color bg);
void terminal_putchar_colored(char c, enum vga_color fg, enum vga_color bg);

void terminal_initialize_history(void);
void terminal_get_history(size_t index, char** line);
void terminal_scroll_up(void);
void terminal_scroll_down(void);
void terminal_page_up(void);
void terminal_page_down(void);
int terminal_get_scroll_position(void);

#endif // TERMINAL_H
