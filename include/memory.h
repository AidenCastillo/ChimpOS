#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

void heap_initialize(void);
void *heap_malloc(size_t size);
void heap_free(void *ptr);

#endif
