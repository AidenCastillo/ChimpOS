#include "graphics.h"
#include "terminal.h"
#include "keyboard.h"

void graphics_demo(void)
{
    /* Initialize graphics system - sets mode 0x13 */
    graphics_initialize();
    
    /* Clear screen to black */
    graphics_clear_screen(0);
    
    /* Fill top third with red */
    for (int y = 0; y < 66; y++) {
        for (int x = 0; x < 320; x++) {
            graphics_set_pixel(x, y, 4);  /* Red */
        }
    }
    
    /* Fill middle third with green */
    for (int y = 66; y < 133; y++) {
        for (int x = 0; x < 320; x++) {
            graphics_set_pixel(x, y, 2);  /* Green */
        }
    }
    
    /* Fill bottom third with blue */
    for (int y = 133; y < 200; y++) {
        for (int x = 0; x < 320; x++) {
            graphics_set_pixel(x, y, 1);  /* Blue */
        }
    }
    
    /* Draw white rectangle in center */
    graphics_fill_rect(100, 75, 120, 50, 7);  /* White */
    
    /* Draw a line across the screen */
    graphics_draw_line(0, 100, 319, 100, 15);  /* Bright white */
    
    /* Wait for 5 seconds with nested loop to prevent optimization */
    for (volatile int j = 0; j < 5; j++) {
        for (volatile int i = 0; i < 100000000; i++) {
            __asm__ volatile("");  /* Prevent optimization */
        }
    }
    
    
    /* Wait for mode switch to complete */
    for (volatile int i = 0; i < 50000000; i++) {
        __asm__ volatile("");
    }
}
