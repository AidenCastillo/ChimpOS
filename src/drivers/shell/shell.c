#include "shell.h"
#include "string.h"
#include "terminal.h"
#include "common.h"

static command_t commands[MAX_COMMANDS];
static int command_count = 0;

int shell_register_command(char* name, void (*function)(int argc, char** argv), char* help) {
    if (command_count >= MAX_COMMANDS) {
        return 0;
    }

    for (int i = 0; i < command_count; i++) {
        if (strcmp(commands[i].name, name) == 0) {
            return 0;
        }
    }

    commands[command_count].name = name;
    commands[command_count].function = function;
    commands[command_count].help = help;
}

int shell_unregister_command(char* name) {
    for (int i = 0; i < command_count; i++) {
        if (strcmp(commands[i].name, name) == 0) {
            for (int j = i; j < command_count - 1; j++) {
                commands[j] = commands[j+1];
            }
            command_count--;
            return 1;
        }
    }

    return 0;
}
static void shell_help(UNUSED int argc, UNUSED char** argv) {
    terminal_writestring("Available commands:\n");
    for (command_t* cmd = commands; cmd->name != NULL; cmd++) {
        terminal_writestring(" ");
        terminal_writestring(cmd->name);
        terminal_writestring(" - ");
        terminal_writestring(cmd->help);
        terminal_writestring("\n");
    }
}

static void shell_echo(int argc, char** argv) {
    terminal_writestring(argv);
    terminal_writestring("\n");
}

void shell_initialize(void) {

    shell_register_command("help", shell_help, "Display this help message");
    shell_register_command("echo", shell_echo, "Echo or prints out the parameter passed");

    terminal_writestring("CHIMP OS SHELL v0.1\n");
    terminal_writestring("Type 'help' for commands\n");
}


void parse_command(char* command_line, int* argc, char* argv) {
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

    for (command_t* cmd = commands; cmd->name != NULL; cmd++) {
        if (strcmp(argv[0], cmd->name) == 0) {
            cmd->function(argc, argv);
            return;
        }
    }

    terminal_writestring("Unknown command: ");
    terminal_writestring(argv[0]);
    terminal_writestring("\n");
}
