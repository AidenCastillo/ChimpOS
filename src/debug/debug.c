#include "debug.h"
#include <stdarg.h>
#include "terminal.h"
#include "string.h"

void debug_init() {
}

void debugf(log_suite_t suite, log_level_t level, const char* fmt, ...) {
    if (level <= suite.level) {
        va_list args;
        va_start(args, fmt);
        debug_parse(level, __FILE__, __LINE__, fmt, args);
        va_end(args);
    }
}

void debug_parse(log_level_t level, const char* file, int line, const char* fmt, va_list args) {
    terminal_writestring("[");
    terminal_writestring(file);
    terminal_writestring(":");
    
    char line_str[12];
    itoa(line, line_str, 10);
    terminal_writestring(line_str);
    terminal_writestring("] ");

    switch(level) {
        case LOG_ERROR:
            terminal_writestring("[E]: ");
            break;
        case LOG_WARN:
            terminal_writestring("[W]: ");
            break;
        case LOG_INFO:
            terminal_writestring("[I]: ");
            break;
        case LOG_VERBOSE:
            terminal_writestring("[V]: ");
            break;
        case LOG_TRACE:
            terminal_writestring("[T]: ");
            break;
        default:
            terminal_writestring("[U]: ");
            break;
    }

    for (const char* p = fmt; *p != '\0'; p++) {
        if (*p != '%') {
            terminal_putchar(*p);
            continue;
        }
        
        p++;
        if (*p == '\0') break;

        switch(*p) {
            case 's': {
                const char* s = va_arg(args, const char*);
                if (s) {
                    terminal_writestring(s);
                } else {
                    terminal_writestring("(NULL)");
                }
                break;
            }
            case 'd': {
                char s[32] = {0};
                int value = va_arg(args, int);
                itoa(value, s, 10);
                terminal_writestring(s);
                break;
            }

            default: {
                terminal_putchar('%');
                terminal_putchar(*p);
            }
        }

    }
    terminal_putchar('\n');
    // No va_end here since we're using a passed va_list
}
