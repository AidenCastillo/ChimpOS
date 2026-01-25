// Attempt to use terminal_writestring to output execution
// ATTENTION: terminal_writestring will cause os crash
// #include <stddef.h>
#include "terminal.h"

int main() {
    // Simple test program
    int sum = 0;
    for (int i = 1; i <= 10; i++) {
        sum += i;
    }
    terminal_writestring("Hello from helloWorld binary!\n");
    return sum; // Should return 55
}
