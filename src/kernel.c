#include <stdbool.h>

#include "terminal.h"
#include "keyboard.h"
#include "shell.h"
void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();
	shell_initialize();
	
	while (true) {
		terminal_writestring("user@host:/$ ");
		terminal_save_position();

		char line[100];
		read_line(line, 100);
		shell_process_command(line);
	}

}
