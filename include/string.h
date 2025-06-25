#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
int strcmp(char* str1, char* str2);
char* strtok(char* str, char* reg);

#endif
