#include <process.h>
#include <stddef.h>
#include <stdint.h>
#include "debug.h"
#include "memory.h"
#include "process.h"
#include "clone.h"

static process_t* current_process = NULL;

process_t* process_init(void) {
    // Initialize process management structures
    current_process = (process_t*)heap_malloc(sizeof(process_t));
    if (current_process == NULL) {
        return NULL;
    }
    current_process->pid = 0;
    current_process->priority = 0;
    current_process->parent = NULL;
    current_process->children = NULL;
    current_process->state = 1; // running
}

static uint32_t get_next_pid(void) {
    static uint32_t next_pid = 1; // Start from 1
    return next_pid++;
}

process_t* process_create_on_node(process_t* parent, uint32_t priority, void (*entry_point)(void)) {
    process_t* new_process = (process_t*)heap_malloc(sizeof(process_t));
    if (new_process == NULL) {
        return NULL;
    }
    new_process->pid = get_next_pid();
    new_process->priority = priority;
    new_process->parent = parent;
    new_process->children = NULL;
    new_process->state = 0;
    return new_process;
}

void process_destroy(uint32_t pid) {
    // Destroy a process
}

void process_schedule(void) {
    // Schedule processes - simple round-robin or priority-based
    // For simplicity, just switch to the next process in the list
    if (current_process != NULL) {
        process_t* next_process = current_process->children;
        if (next_process != NULL) {
            current_process->state = 0;
            next_process->state = 1;
            current_process = next_process;
        }
        else {
            // No other process, continue running current
            current_process->state = 1;
        }
    }
}

int process_wake(process_t* process) {
    int err = 0;
    if (process == NULL) {
        return -1;
    }
    if (process->state == 0) {
        process->state = 1;
        err = 0;
        void* stack = heap_malloc(4096);
        int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID;

        clone(process->function, stack, flags, NULL, NULL, NULL);
    }
    return -1;
}

process_t* process_get_current(void) {
    return current_process;
}
