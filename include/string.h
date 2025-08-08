#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

size_t strlen(const char* str);
int strcmp(char* str1, char* str2);
char* strtok(char* str, char* reg);
void itoa(int num, char* str, uint32_t base);
void strncpy(char* dest, const char* src, size_t n);
char* strdup(const char* str);

#endif
