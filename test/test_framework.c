#include "test_framework.h"
#include <stdio.h>
#include <time.h>

// 测试结果断言
void assert_equal(int expected, int actual, const char *test_name) {
    if (expected != actual) {
        // 失败时的输出，红色并加粗
        printf(COLOR_FAIL "[FAIL] Test %s failed: Expected %d, got %d\n" COLOR_RESET, test_name, expected, actual);
    } else {
        // 成功时的输出，绿色并加粗
        printf(COLOR_PASS "[PASS] Test %s passed\n" COLOR_RESET, test_name);
    }
}

// 运行所有测试并打印详细信息
void run_tests(TestCase *test_cases, size_t num_tests) {
    printf(COLOR_BOLD "Running %zu tests...\n" COLOR_RESET, num_tests);
    size_t failed = 0;
    clock_t start_time = clock(); // 记录总的开始时间

    for (size_t i = 0; i < num_tests; i++) {
        printf("\n--------------------------------------------------\n");
        printf(COLOR_PASS "Running test %zu: %s\n" COLOR_RESET, i, test_cases[i].test_name);

        clock_t test_start = clock();         // 记录当前测试的开始时间
        int result = test_cases[i].test_fn(); // 运行测试函数
        clock_t test_end = clock();           // 记录当前测试的结束时间

        double test_duration = (double)(test_end - test_start) / CLOCKS_PER_SEC; // 测试耗时（秒）

        // 打印测试结果
        if (result == TEST_FAILED) {
            failed++;
            printf(COLOR_FAIL "[FAIL] Test %s failed. Duration: %.6f seconds\n" COLOR_RESET, test_cases[i].test_name,
                   test_duration);
        } else {
            printf(COLOR_PASS "[PASS] Test %s passed. Duration: %.6f seconds\n" COLOR_RESET, test_cases[i].test_name,
                   test_duration);
        }
        printf("--------------------------------------------------\n");
    }

    clock_t end_time = clock();                                               // 记录总的结束时间
    double total_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC; // 总耗时（秒）

    printf("\nAll tests completed.\n");
    printf(COLOR_PASS "Passed Tests: %zu\n" COLOR_RESET, num_tests - failed);
    printf(COLOR_FAIL "Failed Tests: %zu\n" COLOR_RESET, failed);
    printf(COLOR_BOLD "Total time: %.6f seconds\n" COLOR_RESET, total_duration);
}
