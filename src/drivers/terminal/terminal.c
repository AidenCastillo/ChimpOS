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

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
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

void terminal_set_color(uint8_t color) 
{
	terminal_color = color;
}

void terminal_set_fg_color(enum vga_color fg)
{
	uint8_t bg = (terminal_color >> 4) & 0xF;
	terminal_color = vga_entry_color(fg, bg);
}

void terminal_set_bg_color(enum vga_color bg)
{
	uint8_t fg = terminal_color & 0xF;
	terminal_color = vga_entry_color(fg, bg);
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

void terminal_putchar_colored(char c, enum vga_color fg, enum vga_color bg) 
{
	uint8_t saved_color = terminal_color;
	
	terminal_color = vga_entry_color(fg, bg);
	
	terminal_putchar(c);
	
	terminal_color = saved_color;
}

void terminal_write_colored(const char* data, size_t size, enum vga_color fg, enum vga_color bg)
{
	uint8_t saved_color = terminal_color;
	terminal_color = vga_entry_color(fg, bg);
	
	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
	}
	
	terminal_color = saved_color;
}

void terminal_writestring_colored(const char* data, enum vga_color fg, enum vga_color bg)
{
	terminal_write_colored(data, strlen(data), fg, bg);
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
	int codes[10] = {0};
	int code_count = 0;
	int current_code = 0;
	bool parsing_number = false;

	while (j < size && data[j] != 'm' && code_count < 10) {
		if (data[j] >= '0' && data[j] <= '9') {
			current_code = current_code * 10 + (data[j] - '0');
			parsing_number = true;
		} else if (data[j] == ';') {
			if (parsing_number) {
				codes[code_count++] = current_code;
				current_code = 0;
				parsing_number = false;
			}
		}
		j++;
	}

	if (parsing_number) {
		codes[code_count++] = current_code;
	}

	// Process the color codes
	for (int i = 0; i < code_count; i++) {
		int code = codes[i];
		
		// Reset colors
		if (code == 0) {
			terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
		}
		// Foreground colors (30-37)
		else if (code >= 30 && code <= 37) {
			uint8_t fg = (code - 30) & 0x7; // Convert ANSI color to VGA color
			uint8_t bg = (terminal_color >> 4) & 0xF; // Extract current background
			terminal_color = vga_entry_color(fg, bg);
		}
		// Bright foreground colors (90-97)
		else if (code >= 90 && code <= 97) {
			uint8_t fg = ((code - 90) & 0x7) + 8; // Add 8 for bright colors
			uint8_t bg = (terminal_color >> 4) & 0xF; // Extract current background
			terminal_color = vga_entry_color(fg, bg);
		}
		// Background colors (40-47)
		else if (code >= 40 && code <= 47) {
			uint8_t fg = terminal_color & 0xF; // Extract current foreground
			uint8_t bg = (code - 40) & 0x7; // Convert ANSI color to VGA color
			terminal_color = vga_entry_color(fg, bg);
		}
		// Bright background colors (100-107)
		else if (code >= 100 && code <= 107) {
			uint8_t fg = terminal_color & 0xF; // Extract current foreground
			uint8_t bg = ((code - 100) & 0x7) + 8; // Add 8 for bright colors
			terminal_color = vga_entry_color(fg, bg);
		}
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
