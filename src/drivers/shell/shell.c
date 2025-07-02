#include "shell.h"
#include "string.h"
#include "terminal.h"
#include "common.h"
#include "test_framework.h"
#include "memory.h"
static command_list_t command_list = {NULL, 0};
// static int command_count = 0;

int shell_register_command(char* name, void (*function)(int argc, char** argv), char* help) {
    command_node_t* new_command = heap_malloc(sizeof(command_node_t));
    if (!new_command) return 0;

    new_command->name = name;
    new_command->function = function;
    new_command->help = help;

    new_command->next = command_list.head;
    command_list.head = new_command;
    command_list.count++;

    return 1;
}

// int shell_unregister_command(char* name) {
//     for (int i = 0; i < command_count; i++) {
//         if (strcmp(commands[i].name, name) == 0) {
//             for (int j = i; j < command_count - 1; j++) {
//                 commands[j] = commands[j+1];
//             }
//             command_count--;
//             return 1;
//         }
//     }

//     return 0;
// }
static void shell_help(UNUSED int argc, UNUSED char** argv) {
    terminal_writestring("Available commands:\n");
    command_node_t* current = command_list.head;
    // for (command_t* cmd = commands; cmd->name != NULL; cmd++) {
    //     terminal_writestring(" ");
    //     terminal_writestring(cmd->name);
    //     terminal_writestring(" - ");
    //     terminal_writestring(cmd->help);
    //     terminal_writestring("\n");
    // }
    while (current != NULL) {
        terminal_writestring(" ");
        terminal_writestring(current->name);
        terminal_writestring(" - ");
        terminal_writestring(current->help);
        terminal_writestring("\n");
        current = current->next;
    }
}

static void shell_echo(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        terminal_writestring(argv[i]);
        if (i < argc - 1) {
            terminal_writestring(" ");
        }
    }
    terminal_writestring("\n");
}

void shell_initialize(void) {

    shell_register_command("help", shell_help, "Display this help message");
    shell_register_command("echo", shell_echo, "Echo or prints out the parameter passed");
    shell_register_command("test", cmd_run_tests, "Run system tests");

    terminal_writestring("CHIMP OS SHELL v0.1\n");
    terminal_writestring("Type 'help' for commands\n");
}

void parse_command(char* command_line, int* argc, char** argv) {
    *argc = 0;
    char* token = strtok(command_line, " ");

    while (token && *argc < MAX_ARGS) {
        argv[*argc] = token;
        (*argc)++;
        token = strtok(NULL, " ");
    }
}

void shell_process_command(char* command_line) {
    int argc = 0;
    char* argv[MAX_ARGS];

    parse_command(command_line, &argc, argv);

    if (argc == 0) return;

    command_node_t* current = command_list.head;
    while (current != NULL) {
        if (strcmp(argv[0], current->name) == 0) {
            current->function(argc, argv);
            return;
        }
        current = current->next;
    }

    terminal_writestring("Unknown command: ");
    terminal_writestring(argv[0]);
    terminal_writestring("\n");
}
