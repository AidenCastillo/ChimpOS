#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10
#define MAX_COMMANDS 32

#define SHELL_PROMPT "user@host:/$ "
#define SHELL_HISTORY_SIZE 100


typedef struct command_node {
    char* name;
    void (*function)(int argc, char** argv);
    char* help;
    struct command_node* next;
} command_node_t;

typedef struct {
    command_node_t* head;
    size_t count;
} command_list_t;


void shell_initialize(void);
void shell_process_command(char* command_line);
int shell_register_command(char* name, void(*function)(int argc, char** argv), char* help);
int shell_unregister_command(char* name);

#endif
