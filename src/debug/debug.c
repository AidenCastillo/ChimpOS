#include "debug.h"
#include <stdarg.h>

void debug_init() {
}

void debug_log(log_level_t level, const char* file, int line, const char* fmt, ...) {
    // va_list args;
    // va_start(args, fmt);

    // for (const char* p = fmt; *p != '\0'; p++) {
    //     char s = *p;
    //     terminal_writestring(s);

    // }

    // va_end(args);
}
