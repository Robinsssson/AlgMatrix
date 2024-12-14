#include "cso.h"

static double test_function(alg_vector *vec) {
    double ret = 0.f;
    for (int i = 0; i < vec->size; i++) {
        ret += pow(vec->vector[i], 2);
    }
    return ret + 2;
}
#define VAL(v) v
#define VS 50.
int main(void) {
    optim_handle optim;
    double l_range[] = {-VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS), -VAL(VS)},
           r_range[] = {VAL(VS), VAL(VS), VAL(VS), VAL(VS), VAL(VS), VAL(VS)};
    optim_init(&optim, 6, test_function, l_range, r_range);
    cso_handle *handle = cso_init(optim, 210, 30, 60, 120, 1);
    for (int i = 0; i < 50; i++) {
        cso_fresh(handle, 1);
        optim_print(&handle->optim);
    }
    cso_free(handle);
    return 0;
}
