#include <stdbool.h>

#include "terminal.h"
#include "filesystem.h"
#include "keyboard.h"
#include "shell.h"
#include "memory.h"
#include "test_framework.h"
#include "debug.h"
#include "process.h"

void test_function(void) {
	terminal_writestring("Test function running in a new process.\n");
}

void kernel_main(void) 
{
	/* Initializations */
	process_t* current_process = process_init();
	terminal_writestring("Kernel initializing...\n");

	process_t* child_process = process_create_on_node(current_process, 0, test_function);

	process_wake(child_process);

	terminal_initialize();
	debug_init();

	if (fs_init()) {
		LOG_ERROR("Filesystem initialization failed");
	}
	
	heap_initialize();
	shell_initialize();
	test_framework_init();

	int ran_tests = 1;

	while (true) {
		if (ran_tests == 0) {
			shell_process_command("test");
			ran_tests = 1;
			LOG_INFO("TESTS RAN %d", 10);
			LOG_INFO("TEST STRING: %s", "passed");
		}
		terminal_writestring("user@host:/$ ");
		terminal_save_position();
		
		
		char line[100];
		read_line(line, 100);
		shell_process_command(line);
	}

}
