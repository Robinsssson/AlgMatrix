#include "debug_memory.h"
#include "test_framework.h"
#include "test_function_list.h"

int TEST_MAIN(void) {
    alg_memalloc_hook hook = {debug_malloc, debug_free, debug_realloc, debug_calloc};
    alg_memalloc_init(&hook);
    TEST_SCOPE_NEGIN = {
        INSERT_TEST(test_abc), INSERT_TEST(test_aco), INSERT_TEST(test_aia), INSERT_TEST(test_cs),
        INSERT_TEST(test_cso), INSERT_TEST(test_de),  INSERT_TEST(test_eda), INSERT_TEST(test_ga),
        INSERT_TEST(test_pso), INSERT_TEST(test_sa),
    };
    TEST_SCOPE_END
}
