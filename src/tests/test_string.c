#include "test_framework.h"
#include "string.h"
#include "terminal.h"
#include <stddef.h>



static bool test_strcmp() {
    char* str1 = "string";
    char* str2 = "string";
    char* str3 = "characters";
    return (strcmp(str1, str2) == 0 && strcmp(str1, str3) == -1);
}

static bool test_itoa() {
    uint32_t n = 25;
    char* buf[3];
    itoa(n, buf, 10);
    return (strcmp(buf, "25\0") == 0);
}


void register_string_tests(void) {
    test_suite_t* suite = create_test_suite("string");

    add_test_case(suite, "strcmp", "", test_strcmp);
    add_test_case(suite, "itoa", "", test_itoa);

    register_test_suite(suite);
}

REGISTER_TEST_SUITE(register_string_tests)
