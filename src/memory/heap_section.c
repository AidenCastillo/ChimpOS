#include <stdint.h>

// Define a 1MB heap area in BSS (zero-initialized)
__attribute__((aligned(4096)))
unsigned char kernel_heap[1048576];

// These will be accessed from memory.c
void* heap_start = kernel_heap;
void* heap_end = kernel_heap + 1048576;
