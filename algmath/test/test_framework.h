#ifndef __TEST_FRAMEWORK_H
#define __TEST_FRAMEWORK_H

#include <stddef.h>
#define TEST_PASSED 0
#define TEST_FAILED 1

// Test case structure
typedef struct {
    const char *test_name;
    int (*test_fn)(void); // Function pointer to test function
} TestCase;
#define INSERT_TEST(function) {#function, function}
// Assert function to compare expected and actual values
void assert_equal(int expected, int actual, const char *test_name);

// Run all tests
void run_tests(TestCase *test_cases, size_t num_tests);
#define TEST_SCOPE_NEGIN TestCase test_cases[]
#define TEST_SCOPE_END                                                                             \
    size_t num_tests = sizeof(test_cases) / sizeof(TestCase);                                      \
    run_tests(test_cases, num_tests);                                                              \
    return 0;
#define TEST_MAIN main
#endif
