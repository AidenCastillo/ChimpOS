#ifndef SHELL_H
#define SHELL_H

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10
#define MAX_COMMANDS 32

typedef struct {
    char* name;
    void (*function)(int argc, char** argv);
    char* help;
} command_t;

void shell_initialize(void);
void shell_process_command(char* command_line);
int shell_register_command(char* name, void(*function)(int argc, char** argv), char* help);
int shell_unregister_command(char* name);

#endif
