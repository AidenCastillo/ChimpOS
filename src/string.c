#include <stddef.h>

#include "string.h"
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int strcmp(char* str1, char* str2) {
	if (strlen(str1) != strlen(str2)) {
		return -1;
	}

	for (size_t i = 0; i < strlen(str1); i++) {
		if (str1[i] != str2[i]) {
			return -1;
		}
	}

	return 0;
}

char* strchr(const char* str, char c) {
	while (*str) {
		if (*str == c) {
			return (char*)str;
		}
		str++;
	}
	return NULL;
}

static char* next_token = NULL;
char* strtok(char* str, char* reg) {
	if (str != NULL) {
		next_token = str;
	}

	if (next_token == NULL) {
		return NULL;
	}

	char* token_start = next_token;
	while (*next_token && strchr(reg, *next_token) == NULL) {
		next_token++;
	}

	if (*next_token) {
		*next_token = '\0';
		next_token++;
	} else {
		next_token = NULL;
	}

	return token_start;
}
