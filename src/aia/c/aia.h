#ifndef __AIA_H
#define __AIA_H
#include "../../basic_opti.h"

typedef struct {
    int clone_factor, num_antibodies;
    double mutation_rate;
    alg_matrix *population;
    alg_vector *fitness;
    optim_handle optim;
} aia_handle;

ALG_MATH_API aia_handle *aia_init(optim_handle optim, int num_antibodies, int clone_factor, double mutation_rate);
ALG_MATH_API alg_state aia_fresh(aia_handle *handle, int gen);
ALG_MATH_API alg_state aia_free(aia_handle *handle);
#endif
