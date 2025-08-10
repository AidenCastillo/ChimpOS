#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>
#include <stdarg.h>

#ifndef DEBUG
#define DEBUG 0
#endif

typedef enum {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_VERBOSE = 3,
    LOG_TRACE = 4
} log_level_t;

typedef struct log_suite_t {
    log_level_t level;
} log_suite_t;

void debug_init(void);
void debug_set_level(log_level_t level);
void REGISTER_DEBUG_SUITE(log_suite_t suite);

void debug_parse(log_level_t level, const char* file, int line, const char* fmt, va_list args);
void debugf(log_suite_t suite, log_level_t level, const char* fmt, ...);

#define LOG_ERROR(fmt, ...) debugf((log_suite_t){LOG_ERROR}, LOG_ERROR, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) debugf((log_suite_t){LOG_WARN}, LOG_WARN, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) debugf((log_suite_t){LOG_INFO}, LOG_INFO, fmt, ##__VA_ARGS__)
#define LOG_VERBOSE(fmt, ...) debugf((log_suite_t){LOG_VERBOSE}, LOG_VERBOSE, fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) debugf((log_suite_t){LOG_TRACE}, LOG_TRACE, fmt, ##__VA_ARGS__)

#define ASSERT(condition) do { if (!(condition)) {debugf((log_suite_t){LOG_ERROR}, LOG_ERROR, "Assertion failed: %s", #condition); for(;;);}} while(0)

#endif
