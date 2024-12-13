#include "cs.h"

static double test_function(alg_vector *vec) {
    // 确保向量的大小为2（二维空间）
    double ret = 0.f;
    for (int i = 0; i < vec->size; i++) {
        ret += pow(vec->vector[i], 2);
    }
    return ret;
}
#define VAL(v) v
#define VS 50.
int main(void) {
    optim_handle optim;
    double l_range[] = {-VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS)},
           r_range[] = {VAL(VS), VAL(VS), VAL(VS), VAL(VS), VAL(VS), VAL(VS)};
    optim_init(&optim, 6, test_function, l_range, r_range);
    cs_handle *handle = cs_init(optim, 150, 0.01, 0.25, 1.5);
    for (int i = 0; i < 50; i++) {
        cs_fresh(handle, 20);
        optim_print(&handle->optim);
    }
    cs_free(handle);
    return 0;
}
