#include "debug.h"
#include <stdarg.h>
#include "terminal.h"
#include "string.h"

void debug_init() {
}

void debug_log(log_level_t level, const char* file, int line, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    terminal_writestring("[");
    terminal_writestring(file);
    terminal_writestring(":");
    
    char line_str[12];
    itoa(line, line_str, 10);
    terminal_writestring(line_str);
    terminal_writestring("] ");

    for (const char* p = fmt; *p != '\0'; p++) {
        if (*p != '%') {
            terminal_putchar(*p);
            continue;
        }
        
        p++;
        if (*p == '\0') break;

        switch(*p) {
            case 'd': {
                const char s[32];
                itoa(va_arg(args, int*), s, 10);
                if (s) {
                    terminal_writestring(s);
                } else {
                    terminal_writestring("(NULL)");
                }
                break;
            }

            default: {
                terminal_putchar('%');
                terminal_putchar(*p);
            }
        }

    }
    terminal_putchar('\n');
    va_end(args);
}
