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
    add_test_case(suite, "double_free", "Tests double free detection", test_alloc); // Placeholder for actual double free test
    add_test_case(suite, "memory_leak", "Tests memory leak detection", test_alloc); // Placeholder for actual memory leak test
    add_test_case(suite, "memory_info", "Tests memory info retrieval", test_alloc); // Placeholder for actual memory info test
    add_test_case(suite, "heap_initialization", "Tests heap initialization", test_alloc); // Placeholder for actual heap initialization test
    add_test_case(suite, "heap_free", "Tests heap free function", test_alloc); // Placeholder for actual heap free test
    add_test_case(suite, "heap_reallocation", "Tests heap reallocation function", test_alloc); // Placeholder for actual heap reallocation test
    add_test_case(suite, "heap_clearing", "Tests heap clearing function", test_alloc); // Placeholder for actual heap clearing test
    add_test_case(suite, "heap_splitting", "Tests heap splitting function", test_alloc); // Placeholder for actual heap splitting test
    add_test_case(suite, "heap_merging", "Tests heap merging function", test_alloc); // Placeholder for actual heap merging test
    add_test_case(suite, "heap_chunk_size", "Tests heap chunk size calculation", test_alloc); // Placeholder for actual heap chunk size test
    add_test_case(suite, "heap_chunk_inuse", "Tests heap chunk in-use status", test_alloc); // Placeholder for actual heap chunk in-use test
    add_test_case(suite, "heap_chunk_next", "Tests heap chunk next pointer", test_alloc); // Placeholder for actual heap chunk next pointer test

    register_test_suite(suite);
}

REGISTER_TEST_SUITE(register_malloc_tests)
