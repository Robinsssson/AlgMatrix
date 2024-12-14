#ifndef __EDA_H
#define __EDA_H

#include "../basic_opti.h"

typedef struct {
    optim_handle optim;
    int pop_size;
    alg_matrix *population;
    alg_vector *fitness;
    double elitism_rate;
} eda_handle;

ALG_MATH_API eda_handle *eda_init(optim_handle optim, int pop_size, double elitism_rate);
ALG_MATH_API alg_state eda_fresh(eda_handle *handle, int gen);
ALG_MATH_API alg_state eda_free(eda_handle *handle);

#endif
