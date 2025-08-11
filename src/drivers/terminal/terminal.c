#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "terminal.h"
#include "string.h"


/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

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

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

static cursor_position_t prompt_position;

#define HISTORY_SIZE 100
static char* command_history[HISTORY_SIZE];
static size_t history_count = 0;

static uint16_t screen_history[HISTORY_LINES * VGA_WIDTH];
static uint16_t current_screen_buffer[VGA_HEIGHT * VGA_WIDTH];
static int history_start = 0;
static int history_size = 0;
static int scroll_position = 0;

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*)VGA_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
			current_screen_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	
	// Initialize history buffer
	for (size_t i = 0; i < HISTORY_LINES * VGA_WIDTH; i++) {
		screen_history[i] = vga_entry(' ', terminal_color);
	}
	history_start = 0;
	history_size = 0;
	scroll_position = 0;
}

void terminal_initialize_history(void) {
	for (size_t i = 0; i < HISTORY_SIZE; i++) {
		command_history[i] = NULL;
	}
	history_count = 0;
}

void terminal_get_history(size_t index, char** line) {
	if (index < HISTORY_SIZE) {
		*line = command_history[index];
	}
}

int terminal_get_scroll_position(void) {
	return scroll_position;
}

void terminal_get_position(cursor_position_t* pos) {
	pos->row = terminal_row;
	pos->column = terminal_column;
}

void terminal_save_position(void) {
	prompt_position.row = terminal_row;
	prompt_position.column = terminal_column;
}

cursor_position_t terminal_get_prompt_position(void) {
    return prompt_position;
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void save_screen_to_history(void) {
	if (history_size >= HISTORY_LINES) {
		history_start = (history_start + 1) % HISTORY_LINES;
	} else {
		history_size++;
	}
	
	int save_index = (history_start + history_size - 1) % HISTORY_LINES;
	
	for (int x = 0; x < VGA_WIDTH; x++) {
		screen_history[save_index * VGA_WIDTH + x] = terminal_buffer[x];
	}
}

void terminal_scroll() {
	if (scroll_position == 0) {
		save_screen_to_history();
	}
	
	for (int y = 0; y < VGA_HEIGHT - 1; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			const size_t dest_index = y * VGA_WIDTH + x;
			const size_t src_index = (y + 1) * VGA_WIDTH + x;
			terminal_buffer[dest_index] = terminal_buffer[src_index];
		}
	}
	
	for (int x = 0; x < VGA_WIDTH; x++) {
		terminal_putentryat(' ', terminal_color, x, VGA_HEIGHT - 1);
	}
}

// Save the current screen content before scrolling
void save_current_screen() {
	for (int y = 0; y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			current_screen_buffer[index] = terminal_buffer[index];
		}
	}
}

void refresh_screen_from_history() {
	if (scroll_position == 0) {
		// Restore the current screen
		for (int y = 0; y < VGA_HEIGHT; y++) {
			for (int x = 0; x < VGA_WIDTH; x++) {
				const size_t index = y * VGA_WIDTH + x;
				terminal_buffer[index] = current_screen_buffer[index];
			}
		}
		return;
	}
	
	int available_scroll = (scroll_position > history_size) ? history_size : scroll_position;
	
	for (int y = 0; y < VGA_HEIGHT; y++) {
		if (y < available_scroll) {
			int history_line = (history_start + history_size - available_scroll + y) % HISTORY_LINES;
			
			for (int x = 0; x < VGA_WIDTH; x++) {
				terminal_buffer[y * VGA_WIDTH + x] = screen_history[history_line * VGA_WIDTH + x];
			}
		} else {
			int src_line = y - available_scroll;
			if (src_line < VGA_HEIGHT) {
				for (int x = 0; x < VGA_WIDTH; x++) {
					terminal_buffer[y * VGA_WIDTH + x] = current_screen_buffer[src_line * VGA_WIDTH + x];
				}
			}
		}
	}
}

void terminal_page_up(void) {
	if (scroll_position == 0) {
		save_current_screen();
	}
	
	int scroll_amount = VGA_HEIGHT / 2;
	
	int max_scroll = history_size - scroll_position;
	if (scroll_amount > max_scroll) {
		scroll_amount = max_scroll;
	}
	
	if (scroll_amount > 0) {
		scroll_position += scroll_amount;
		refresh_screen_from_history();
	}
}

void terminal_page_down(void) {
	if (scroll_position > 0) {
		int scroll_amount = VGA_HEIGHT / 2;
		
		if (scroll_amount > scroll_position) {
			scroll_amount = scroll_position;
		}
		
		scroll_position -= scroll_amount;
		refresh_screen_from_history();
	}
}

void terminal_scroll_up(void) {
	if (scroll_position == 0) {
		save_current_screen();
	}
	
	if (scroll_position < history_size) {
		scroll_position++;
		refresh_screen_from_history();
	}
}

void terminal_scroll_down(void) {
	if (scroll_position > 0) {
		scroll_position--;
		refresh_screen_from_history();
	}
}

void terminal_putchar(char c) 
{
	if (scroll_position > 0) {
		scroll_position = 0;
		for (int y = 0; y < VGA_HEIGHT; y++) {
			for (int x = 0; x < VGA_WIDTH; x++) {
				const size_t index = y * VGA_WIDTH + x;
				terminal_buffer[index] = current_screen_buffer[index];
			}
		}
	}

	switch(c) {
		case NEWLINE:
			{
				terminal_column = 0;
				terminal_row++; 
				break;
			}
		case BACKSPACE_CHAR:
			{
				cursor_position_t current = {terminal_row, terminal_column};
				cursor_position_t prompt = terminal_get_prompt_position();

				if (current.row > prompt.row || (current.row == prompt.row && current.column > prompt.column)) {
					if (terminal_column > 0) {
						terminal_column--;
						terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
					} else if (terminal_row > 0) {
						terminal_row--;
						terminal_column = VGA_WIDTH - 1;
						terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
					}
				}
				break;
			}
		default:
			{
				terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
				if (++terminal_column == VGA_WIDTH) {
					terminal_column = 0;
					terminal_row++;
				}
				break;
			}
	}

	if (terminal_row == VGA_HEIGHT) {
		terminal_row--;
		terminal_scroll();
	}
	
	if (scroll_position == 0) {
		for (int y = 0; y < VGA_HEIGHT; y++) {
			for (int x = 0; x < VGA_WIDTH; x++) {
				const size_t index = y * VGA_WIDTH + x;
				current_screen_buffer[index] = terminal_buffer[index];
			}
		}
	}
}

static void handle_escape_sequence(const char* data, size_t* index, size_t size) {
	if (*index + 1 >= size || data[*index + 1] != CSI_START) {
		return;
	}

	size_t j = *index + 2;
	uint16_t code[4] = {0};
	int code_index = 0;

	while (j < size && data[j] != 'm' && code_index < 3) {
		code[code_index++] = data[j++] - '0';
	}

	int color_code = 0;
	for (int i = 0; i < code_index; i++) {
		color_code = color_code * 10 + code[i];
	}

	switch (color_code) {
		case 94: terminal_color = vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK);
	}

	*index = j;
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++) {
		if (data[i] == ESCAPE_CHAR) {
			handle_escape_sequence(data, &i, size);
			continue;
		}
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void terminal_clear(void) {
	terminal_row = 0;
	terminal_column = 0;
	scroll_position = 0;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	prompt_position.row = 0;
	prompt_position.column = 0;
}
