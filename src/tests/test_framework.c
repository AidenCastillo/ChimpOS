#include "test_framework.h"
#include "terminal.h"
#include "memory.h"
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

void add_test_case(test_suite_t* suite, const char* name, const char* desc, bool(*test_func)(void)) {
    test_case_t* test = heap_malloc(sizeof(test_case_t));

    test->name = name;
    test->description = desc;
    test->test_func = test_func;

    if (suite->tests == NULL) {
        suite->tests = heap_malloc(sizeof(test_case_t));
        suite->tests[0] = *test;
        heap_free(test);
        suite->test_count = 1;
    } else {
        test_case_t* new_array = heap_malloc(sizeof(test_case_t) * (suite->test_count + 1));

        for (int i = 0; i < suite->test_count; i++) {
            new_array[i] = suite->tests[i];
        }

        new_array[suite->test_count] = *test;

        heap_free(suite->tests);
        heap_free(test);

        suite->tests = new_array;
        suite->test_count++;    
    }

}

void register_test_suite(test_suite_t* suite) {
    suite->next = test_registry.head;
    test_registry.head = suite;
    test_registry.suite_count++;
}

test_suite_t* get_test_suite(const char* name) {
    test_suite_t* current = test_registry.head;
    while (current) {
        if (strcmp(current->suite_name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

test_case_t* get_test_case(const char* suite, const char* name) {
    test_suite_t* current = get_test_suite(suite);
    if (!current) return NULL;

    for (int i = 0; i < current->test_count; i++) {
        if (strcmp(current->tests[i].name, name) == 0) {
            return &current->tests[i];
        }
    }
    return NULL;
}

void run_test_suite(test_suite_t* suite) {
    terminal_writestring("\n=== Running test suite: ");
    terminal_writestring(suite->suite_name);
    terminal_writestring(" ===\n");

    if (suite->test_count == 0) {
        terminal_writestring("No tests in suite\n");
    } else {
        for (int i = 0; i < suite->test_count; i++) {
            test_case_t* test = &suite->tests[i];
            run_test_case(test);
        }
    }
}

int run_test_case(test_case_t* test) {
    if (!test) return -1;

    terminal_writestring("  - ");
    terminal_writestring(test->name);
    terminal_writestring(": ");

    bool result = test->test_func();

    if (result) {
        terminal_writestring("PASS\n");
        test_registry.passed++;
        return 1;
    } else {
        terminal_writestring("FAIL\n");
        test_registry.failed++;
        return 0;
    }
}

void cmd_run_tests(int argc, char** argv) {
    
    terminal_writestring("\n");
    terminal_writestring("=== TEST FRAMEWORK ===\n");
    
    if (argc > 2) {
        if (strcmp(argv[1], "-s") == 0) {
            if (argc < 3) {
                terminal_writestring("No test suite specified\n");
                return;
            }
            test_suite_t* suite = get_test_suite(argv[2]);
            if (suite) {
                if (argc > 3) {
                    for (int i = 4; i < argc; i++) {
                        test_case_t* test = get_test_case(argv[2], argv[i]);
                        if (test) {
                            run_test_case(test);
                        } else {
                            terminal_writestring("Test case not found\n");
                        }
                    }
                } else {
                    run_test_suite(suite);
                }
            } else {
                terminal_writestring("Test suite not found\n");
            }
        } else {
            terminal_writestring("Invalid test argument\n");
        }
    } else {
        run_all_tests();
    }

    terminal_writestring("\n");
    terminal_writestring("=== PASSED: ");
    char buf[32];
    itoa(test_registry.passed, buf, 10);
    terminal_writestring(buf);
    terminal_writestring("\n=== FAILED: ");
    itoa(test_registry.failed, buf, 10);
    terminal_writestring(buf);
    terminal_writestring("\n");

    test_registry.passed = 0;
    test_registry.failed = 0;

    // test_framework_cleanup(); 
    return;
}

void run_all_tests() {
    test_suite_t* current_suite = test_registry.head;

    test_registry.passed = 0;
    test_registry.failed = 0;

    while (current_suite) {
        run_test_suite(current_suite);

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
