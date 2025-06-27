#include <stdbool.h>

#include "terminal.h"
#include "keyboard.h"
#include "shell.h"
#include "memory.h"
void kernel_main(void) 
{
	/* Initialize terminal interface */
	heap_initialize();
	terminal_initialize();
	shell_initialize();

	void* ptr = heap_malloc(32);
	if (ptr == NULL) {
		terminal_writestring("Memory allocation failed!\n");
	} else {
		terminal_writestring("Memory allocation succeeded!\n");
	}
	
	while (true) {
		terminal_writestring("user@host:/$ ");
		terminal_save_position();

		char line[100];
		read_line(line, 100);
		shell_process_command(line);
	}

}
