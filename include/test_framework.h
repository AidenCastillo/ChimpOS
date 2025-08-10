#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct test_case_t {
    const char* name;
    const char* description;
    bool (*test_func)(void);
} test_case_t;

struct test_suite_node;

typedef struct test_suite_node {
    const char* suite_name;
    test_case_t* tests;
    int test_count;
    struct test_suite_node* next;
} test_suite_t;

typedef struct {
    test_suite_t* head;
    int suite_count;
    uint32_t passed;
    uint32_t failed;
} test_registry_t;

extern test_registry_t test_registry;

#define REGISTER_TEST_SUITE(func) \
    __attribute__((section(".test_registry"))) \
    void (*__test_reg_##func)(void) = func;

#define TEST_HELP_STRING_GENERAL "Run all registered tests or specific test suites"

void cmd_run_tests(int argc, char** argv);

void test_framework_init(void);
void test_framework_cleanup(void);
test_suite_t* create_test_suite(const char* name);
void register_test_suite(test_suite_t* suite);
void add_test_case(test_suite_t* suite, const char* name, const char* desc, bool (*test_func)(void));
test_suite_t* get_test_suite(const char* name);
test_case_t* get_test_case(const char* suite_name, const char* test_name);
// test_result_t* get_test_result(const char* suite_name, const char* test_name);

void run_test_by_name(const char* suite_name);
void run_test_suite(test_suite_t* suite);
void run_all_tests(void);

#endif // TEST_FRAMEWORK_H
