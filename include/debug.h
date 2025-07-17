#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

typedef enum {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_VERBOSE = 3,
    LOG_TRACE = 4
} log_level_t;

void debug_init(void);
void debug_set_level(log_level_t level);

void debug_log(log_level_t level, const char* file, int line, const char* fmt, ...);

#define LOG_ERROR(fmt, ...) debug_log(LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) debug_log(LOG_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) debug_log(LOG_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_VEROSE(fmt, ...) debug_log(LOG_VERBOSE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) debug_log(LOG_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define ASSERT(condition) do { if (!condition) {debug_log(LOG_ERROR, __FILE__, __LINE__, "Assertion failed: %s", #condition); for(;;);}} while(0)

#endif
