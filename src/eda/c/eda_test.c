#include "eda.h"

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
    eda_handle *handle = eda_init(2, 50, -100, 100, 0.2, test_function);
    for (int i = 0; i < 51; i++) {
        eda_fresh(handle);
        TESTLOG_ARGS("GEN %d BEST SOLVE %f", i, handle->best_val);
    }
    char *prs = alg_vector_print_str(handle->best_solve);
    TESTLOG_ARGS("BEST SOLVE IS %s, BEST VAL IS %f", prs, handle->best_val);
    ALG_FREE(prs);
    eda_free(handle);
    return 0;
}
