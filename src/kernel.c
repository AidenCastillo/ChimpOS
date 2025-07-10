#include <stdbool.h>

#include "terminal.h"
#include "filesystem.h"
#include "keyboard.h"
#include "shell.h"
#include "memory.h"
#include "test_framework.h"

void kernel_main(void) 
{
	/* Initializations */
	terminal_initialize();
	fs_init();
	heap_initialize();
	shell_initialize();
	test_framework_init();

	int ran_tests = 0;

	while (true) {
		if (ran_tests == 0) {
			shell_process_command("test");
			ran_tests = 1;
		}
		terminal_writestring("user@host:/$ ");
		terminal_save_position();
		
		
		char line[100];
		read_line(line, 100);
		shell_process_command(line);
	}

}
