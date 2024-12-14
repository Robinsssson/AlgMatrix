#ifndef __ABC_H
#define __ABC_H

#include "../basic_opti.h"

typedef struct {
    optim_handle optim;
    int pop_size, max_count;
    alg_matrix *population;
    alg_vector *fitness;
    int *count;
} abc_handle;

ALG_MATH_API abc_handle *abc_init(optim_handle optim, int pop_size, int max_count);
ALG_MATH_API alg_state abc_fresh(abc_handle *handle, int gen);
ALG_MATH_API alg_state abc_free(abc_handle *handle);
#endif
