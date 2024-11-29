#include "alg_utils.h"
#include "../alg_inc.h"
#include <math.h>

int alg_utils_greater(const void *a, const void *b) {
    alg_val_type val = *(alg_val_type *)a - *(alg_val_type *)b;
    if (fabs(val) < 1e-9)
        return 0;
    else
        return (int)(val);
}

int alg_utils_less(const void *a, const void *b) {
    alg_val_type val = *(alg_val_type *)b - *(alg_val_type *)a;
    if (fabs(val) < 1e-9)
        return 0;
    else
        return (int)(val);
}