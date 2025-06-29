#include "test_framework.h"
#include "terminal.h"
#include "string.h"

test_registry_t test_registry;
extern void (*__test_registry_start)(void);
extern void (*__test_registry_end)(void);

void test_framework_init() {
    test_registry.head = NULL;


    void (**func_ptr)(void) = &__test_registry_start;
    while (func_ptr < &__test_registry_end) {
        (*func_ptr)();
        func_ptr++;
    }
}

test_suite_t* create_test_suite(const char* name) {
    test_suite_t* suite = heap_malloc(sizeof(test_suite_t));

    suite->suite_name = name;
    suite->tests = NULL;
    suite->test_count = 0;
    suite->next = NULL;

    return suite;
}

void add_test_case(test_suite_t* suite, const char* name, const char* desc, bool(*test_func)) {
    test_case_t* test = heap_malloc(sizeof(test_case_t));

    test->name = name;
    test->description = desc;
    test->test_func = test_func;

    if (suite->tests == NULL) {
        suite->tests = heap_malloc(sizeof(test_case_t));
        suite->test_count++;
        suite->tests[0] = *test;
        heap_free(test);
    } else {
        test_case_t* new_array = heap_malloc(sizeof(test_case_t) * (suite->test_count + 1));

        for (int i = 0; i < suite->test_count; i++) {
            new_array[i] = suite->tests[i];
        }

        new_array[suite->test_count] = *test;

        heap_free(suite->tests);
        heap_free(test);

        suite->tests = new_array;
    }

    suite->test_count;
}

void register_test_suite(test_suite_t* suite) {
    suite->next = test_registry.head;
    test_registry.head = suite;
    test_registry.suite_count++;
}

void cmd_run_tests(int argc, char** argv) {

    terminal_writestring("=== TEST FRAMEWORK ===\n");

    run_all_tests();

    terminal_writestring("=== PASSED: ");
    char buf[32];
    itoa(test_registry.passed, buf, 10);
    terminal_writestring(buf);
    terminal_writestring("\n=== FAILED: ");
    itoa(test_registry.failed, buf, 10);
    terminal_writestring(buf);
    terminal_writestring("\n");

    // test_framework_cleanup(); 
    return;
}

void run_all_tests() {
    test_suite_t* current_suite = test_registry.head;

    test_registry.passed = 0;
    test_registry.failed = 0;

    while (current_suite) {
        terminal_writestring("\n=== Running test suite: ");
        terminal_writestring(current_suite->suite_name);
        terminal_writestring(" ===\n");
        
        if (current_suite->test_count == 0) {
            terminal_writestring("No tests in suite\n");
        } else {
            // Run each test in the suite
            for (int i = 0; i < current_suite->test_count; i++) {
                test_case_t* test = &current_suite->tests[i];
                
                terminal_writestring("  - ");
                terminal_writestring(test->name);
                terminal_writestring(": ");
                
                // Run the test function
                bool result = test->test_func();
                
                if (result) {
                    terminal_writestring("PASS\n");
                    test_registry.passed++;
                } else {
                    terminal_writestring("FAIL\n");
                    test_registry.failed++;
                }
            }
        }
        
        // Move to next suite
        current_suite = current_suite->next;
    }

    return;
}

void test_framework_cleanup() {
    test_suite_t* current = test_registry.head;
    
    while (current) {
        test_suite_t* to_free = current;
        current = current->next;
        
        // Free the tests array
        if (to_free->tests) {
            heap_free(to_free->tests);
        }
        
        // Free the test suite itself
        heap_free(to_free);
    }
    
    // Reset the registry
    test_registry.head = NULL;
    test_registry.suite_count = 0;
}
