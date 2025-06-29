#include "test_framework.h"
#include "memory.h"
#include "terminal.h"

static bool test_alloc() {
    void* ptr = heap_malloc(32);
    bool result = (ptr != NULL);
    return result;
}

void register_malloc_tests(void) {
    test_suite_t* suite = create_test_suite("malloc");

    add_test_case(suite, "basic_allocation", "Tests allocation functions", test_alloc);

    register_test_suite(suite);
}

REGISTER_TEST_SUITE(register_malloc_tests)
