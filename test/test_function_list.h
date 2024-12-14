#ifndef __TEST_FUNCTION_LIST__H__
#define __TEST_FUNCTION_LIST__H__

#include "../src/optimization.h"

int test_abc(void);
int test_aco(void);
int test_aia(void);
int test_cs(void);
int test_cso(void);
int test_de(void);
int test_eda(void);
int test_ga(void);
int test_pso(void);
int test_sa(void);
#define TEST_FUNC 0
extern optimization test_function_list[];
extern double test_return_list[];
#endif  //!__TEST_FUNCTION_LIST__H__
