#include "memory.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "terminal.h"
#include "string.h"

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
    terminal_writestring("Verifying heap boundaries...\n");
    
    // Print addresses of symbol locations first
    terminal_writestring("heap_start symbol location: 0x");
    char buf[32];
    itoa((unsigned int)&heap_start, buf, 16);  // Address of the symbol itself
    terminal_writestring(buf);
    
    terminal_writestring("\nheap_start value: 0x");
    itoa((unsigned int)heap_start, buf, 16);   // Value the symbol points to
    terminal_writestring(buf);

    if ((void*)heap_start == NULL || (void*)heap_end == NULL) {
        terminal_writestring("\nERROR: Heap boundaries are NULL\n");
        return;
    }

    terminal_writestring("\nHeap start address: 0x");
    itoa((unsigned int)&heap_start, buf, 16);
    terminal_writestring(buf);
    terminal_writestring("\nHeap end address: 0x");
    itoa((unsigned int)&heap_end, buf, 16);
    terminal_writestring(buf);
    terminal_writestring("\n");

    heap_info.available = (uint32_t)((char*)heap_end - (char*)heap_start);
    
    terminal_writestring("Calculated size: ");
    itoa(heap_info.available, buf, 10);
    terminal_writestring(buf);
    terminal_writestring(" bytes\n");
    
    heap_info.start = (struct heapchunk_t*)heap_start;
    if (heap_info.start == NULL) {
        terminal_writestring("ERROR: heap_start is NULL\n");
        return;
    }
    terminal_writestring("heap_info.start address: 0x");
    itoa((unsigned int)heap_info.start, buf, 16);
    terminal_writestring(buf);
    terminal_writestring("\n");

    heap_info.start->size = heap_info.available - sizeof(struct heapchunk_t);
    heap_info.start->inuse = false;
    heap_info.start->next = NULL;
    terminal_writestring("Initial block size: ");
    itoa(heap_info.start->size, buf, 10);
    terminal_writestring(buf);
    terminal_writestring(" bytes\n");
}

void* heap_malloc(size_t size) {
    if (size > heap_info.available) {
        terminal_writestring("Not enough available memory (");
        char buf[32];
        itoa(heap_info.available, buf, 10);
        terminal_writestring(buf);
        terminal_writestring(" bytes available)\n");
        return NULL;
    }

    struct heapchunk_t *current = heap_info.start;
    while (current != NULL) {

        if (!current->inuse && current->size >= size) {
            // Split block if there's enough space for a new chunk
            if (current->size > size + sizeof(struct heapchunk_t)) {
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

    terminal_writestring("No suitable memory chunk found\n");
    return NULL;
}

void heap_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    struct heapchunk_t *chunk = (struct heapchunk_t *)((char *)ptr - sizeof(struct heapchunk_t));
    if (!chunk->inuse) {
        terminal_writestring("Double free detected");
        return;
    }
    chunk->inuse = false;
    heap_info.available += (chunk->size + sizeof(struct heapchunk_t));

    struct heapchunk_t *current = heap_info.start;
    while (current != NULL) {
        if (!current->inuse && current->next != NULL && !current->next->inuse) {
            current->size += sizeof(struct heapchunk_t) + current->next->size;
            current->next = current->next->next;
            continue;
        }
        current = current->next;
    }
}
