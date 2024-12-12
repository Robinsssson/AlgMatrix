#include "de.h"
#include "utils/alg_utils.h"
#include "vector/alg_vector.h"

static int select_best_solve(de_handle *handle) {
    return alg_vector_compare_val(handle->fitness, alg_utils_greater);
}

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
    de_handle *handle = de_init(50, 2, -5, 5, test_function, 0.8, 0.9);
    int idx = select_best_solve(handle);
    for (int i = 1; i < 101; i++) {
        de_fresh(handle);
        TESTLOG_ARGS("GEN %d BEST SOLVE %f", i, handle->fitness->vector[idx]);
    }
    alg_vector *best_solve = alg_vector_from_matrix_row(handle->population, idx);
    char *prs = alg_vector_print_str(best_solve);
    TESTLOG_ARGS("BEST SOLVE IS %s, BEST VAL IS %f", prs, handle->fitness->vector[idx]);
    ALG_FREE(prs);
    de_free(handle);
    return 0;
}
