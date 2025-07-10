#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

void heap_initialize(void);
void* heap_malloc(size_t size);
void heap_free(void *ptr);

void memcpy(void *to, void *from, uint32_t numBytes);
void memset(void *ptr, int value, size_t numBytes);

#endif
