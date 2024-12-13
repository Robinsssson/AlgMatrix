#ifndef __CS_H
#define __CS_H

#include "../../basic_opti.h"
#include "alg_inc.h"

typedef struct {
    optim_handle optim;
    double step_size, pa, lambda;
    int pop_size;
    alg_matrix *population;
    alg_vector *fitness;
} cs_handle;

cs_handle *cs_init(optim_handle optim, int pop_size, double step_size, double pa, double lambda);
alg_state cs_fresh(cs_handle *handle, int gen);
alg_state cs_free(cs_handle *handle);

#endif // !__CS_H
