#include "aia.h"
#include "alg_inc.h"
#include "memalloc/alg_memalloc.h"
#include "vector/alg_vector.h"

static double test_function(alg_vector *vec) {
    // 确保向量的大小为2（二维空间）
    if (vec == NULL || vec->size != 2) {
        return -1;
    }

    double x = vec->vector[0];
    double y = vec->vector[1];

    // 计算目标函数 f(x, y) = (x - 1)^2 + (y - 2)^2
    return pow(x - 1, 2) + pow(y - 2, 2);
}

int main(void) {
    aia_handle *handle = aia_init(50, 3, 2, 0.2, -5, 5, test_function);
    for (int i = 0; i < 500; i++) {
        aia_fresh(handle);
        TESTLOG_ARGS("GEN %d BEST SOLVE %f", i, handle->best_fitness);
    }
    char *prs = alg_vector_print_str(handle->best_solve);
    TESTLOG_ARGS("BEST SOLVE IS %s, BEST VAL IS %f", prs, handle->best_fitness);
    ALG_FREE(prs);
    aia_free(handle);
    return 0;
}
