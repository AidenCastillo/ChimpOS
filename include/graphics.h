#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stddef.h>

/* Graphics video modes */
#define VBE_MODE_640x480_8bit   0x101
#define VBE_MODE_800x600_8bit   0x103
#define VBE_MODE_1024x768_8bit  0x105

/* Current graphics mode info (passed from bootloader) */
typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t pitch;         /* bytes per line */
    uint8_t bpp;            /* bits per pixel */
    uint32_t frame_buffer;  /* physical address of frame buffer */
    uint8_t mode;           /* graphics mode set */
} graphics_mode_t;

extern graphics_mode_t current_graphics_mode;

extern uint8_t* front_buffer;
extern uint8_t* back_buffer;

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_BROWN 14
#define WHITE 15

// Source - https://stackoverflow.com/a/23130671
// Posted by nimbus_debug
// Retrieved 2026-06-04, License - CC BY-SA 3.0

extern unsigned char bit13_letters[95][13]; // 95 printable ASCII characters, each 8x13 pixels
extern unsigned char bit8_letters[95][8];
extern unsigned char bit6_letters[95][6];
/* Function declarations */
void graphics_initialize(void);
void graphics_set_pixel(uint32_t x, uint32_t y, uint8_t color);
void graphics_fill_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t color);
void graphics_draw_line(int x0, int y0, int x1, int y1, uint8_t color);
void graphics_clear_screen(uint8_t color);
void graphics_set_palette_color(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void graphics_exit_to_text_mode(void);

/* Palette helper functions */
void graphics_setup_palette(void);

void graphics_draw_char(char c, uint32_t x, uint32_t y, uint8_t color);
void graphics_draw_char_8x8(char c, uint32_t x, uint32_t y, uint8_t color);
void graphics_draw_char_4x6(char c, uint32_t x, uint32_t y, uint8_t color);
void graphics_swap_buffers(void);

#endif
