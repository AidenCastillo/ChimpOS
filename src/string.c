#include "string.h"

#include "common.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

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

void itoa(int n, char* str, UNUSED uint32_t base) {
    int i = 0;
    
    // Store the sign but work with positive numbers
    bool isNegative = (n < 0);
    uint32_t num = isNegative ? -n : n;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Process digits
    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Add sign if negative
    if (isNegative) {
        str[i++] = '-';
    }

    // Null terminate
    str[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i/2; j++) {
        char temp = str[j];
        str[j] = str[i-1-j];
        str[i-1-j] = temp;
    }
}
