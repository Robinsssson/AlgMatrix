#include "../../src/optimization.h"
#include "../debug_memory.h"
#include "../test_framework.h"
#include "../test_function_list.h"

#define VAL(v) v
#define VS 50.
#define OPTIM_METHOD eda
#define OPTIM_METHOD_ARGS 50, 0.2

#define CONCATENATE(x, y) x##y
#define EXPAND_AND_CONCATENATE(x, y) CONCATENATE(x, y)
#define HANDLE(method) EXPAND_AND_CONCATENATE(method, _handle)
#define INIT(method) EXPAND_AND_CONCATENATE(method, _init)
#define FRESH(method) EXPAND_AND_CONCATENATE(method, _fresh)
#define FREE(method) EXPAND_AND_CONCATENATE(method, _free)
#define TEST_FUNCTION(method) EXPAND_AND_CONCATENATE(test_, method)

int TEST_FUNCTION(OPTIM_METHOD)(void) {
    optim_handle optim;
    double l_range[] = {-VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS)},
           r_range[] = {VAL(VS), VAL(VS), VAL(VS), VAL(VS), VAL(VS), VAL(VS)};
    optim_init(&optim, 6, test_function_list[TEST_FUNC], l_range, r_range);
    HANDLE(OPTIM_METHOD) *handle = INIT(OPTIM_METHOD)(optim, OPTIM_METHOD_ARGS);
    for (int i = 0; i < 50; i++) {
        FRESH(OPTIM_METHOD)(handle, 50);
        optim_print(&handle->optim);
    }
    if (FLOAT_COMPARE_IS(handle->optim.bast_value, test_return_list[TEST_FUNC])) {
        FREE(OPTIM_METHOD)(handle);
        optim_free(&optim);
        check_memory_leaks();
        return TEST_PASSED;
    } else {
        FREE(OPTIM_METHOD)(handle);
        optim_free(&optim);
        check_memory_leaks();
        return TEST_FAILED;
    }
}
