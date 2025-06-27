#include "memory.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "terminal.h"

extern void* heap_start;
extern void* heap_end;

struct heapinfo_t {
    uint32_t available;
    struct heapchunk_t *start;
};
struct heapchunk_t {
    uint32_t size;
    bool inuse;
    struct heapchunk_t *next;
};

static struct heapinfo_t heap_info;

void heap_initialize(void) {
    heap_info.available = (uint32_t)((char*)heap_end - (char*)heap_start);
    heap_info.start = heap_start;

    heap_info.start->size = heap_info.available - sizeof(struct heapchunk_t);
    heap_info.start->inuse = false;
    heap_info.start->next = NULL;
}

void* heap_malloc(size_t size) {
    if (size > heap_info.available) {
        terminal_writestring("Not enough available memory");
        return NULL;
    }

    struct heapchunk_t *current = heap_info.start;
    while (current != NULL) {
        if (!current->inuse && current->size >= size) {
            if (current->size >= size + sizeof(struct heapchunk_t) + 1) {
                struct heapchunk_t *new_chunk = (struct heapchunk_t *)((char *)current + sizeof(struct heapchunk_t) + size);
                new_chunk->size = current->size - size - sizeof(struct heapchunk_t);
                new_chunk->inuse = false;
                new_chunk->next = current->next;

                current->size = size;
                current->next = new_chunk;
            }
            current->inuse = true;
            heap_info.available -= (current->size + sizeof(struct heapchunk_t));
            return (char *)current + sizeof(struct heapchunk_t);
        }
        current = current->next;
    }
    terminal_writestring("No suitable memory chunk found");
    return NULL;
}

