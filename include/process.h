#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
typedef struct {
    uint32_t pid;
    uint32_t priority;
    struct process_t* parent; // Pointer to the parent process
    struct process_t* children; // Pointer to the list of child processes
    int state; // waiting=0, running=1, stopped=2
    void (*function)(void); // Function to execute
    // Other process-related information
} process_t;

process_t* process_init(void); // Initialize process management
process_t* process_create_on_node(process_t* parent, uint32_t priority, void (*entry_point)(void)); // Create a new process
void process_destroy(uint32_t pid); // Destroy a process
void process_schedule(void); // Schedule processes - simple round-robin or priority-based
process_t* process_get_current(void); // Get the currently running process
int process_wake(process_t* process); // Wake up a sleeping process

#endif // PROCESS_H
