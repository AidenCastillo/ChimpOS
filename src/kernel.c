#include <stdbool.h>

#include "terminal.h"
#include "keyboard.h"

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();

	/* Newline support is left as an exercise. */
	// terminal_writestring("Hello, ke\033[094mrnel World!WorldWorldWorldWorldWorldWorldWorldWorldWorldWorldWorldWorldWorld\n");
	// for (int i = 0; i < 22; i++) {
	// 	terminal_writestring("Hello, kernel World!\n");
	// }
	
	while (true) {
		terminal_writestring("user@host:/$ ");
		terminal_save_position();

		char line[100];
		read_line(line, 100);
	}

}
