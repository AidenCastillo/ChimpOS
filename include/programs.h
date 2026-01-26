#ifndef PROGRAMS_H
#define PROGRAMS_H

#include <stdint.h>

extern uint8_t helloWorld_bin_start[];
extern uint8_t helloWorld_bin_end[];

extern uint8_t terminalProg_bin_start[];
extern uint8_t terminalProg_bin_end[];

typedef struct {
    const char* name;
    uint8_t* start;
    uint8_t* end;
} program_t;

static const program_t programs[] = {
    { "helloworld", helloWorld_bin_start, helloWorld_bin_end },
    { "terminalProg", terminalProg_bin_start, terminalProg_bin_end },
    { NULL, NULL, NULL }
};

#define PROGRAMS_COUNT (sizeof(programs) / sizeof(program_t) - 1)

#endif // PROGRAMS_H
