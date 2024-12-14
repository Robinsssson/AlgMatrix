#ifndef __DE_H
#define __DE_H

#include "../../basic_opti.h"

typedef struct {
    int pop_size;
    double f;
    double cr;
    alg_matrix *population;
    alg_vector *fitness;
    optim_handle optim;
} de_handle;

ALG_MATH_API de_handle *de_init(optim_handle optim, int pop_size, double f, double cr);
ALG_MATH_API alg_state de_fresh(de_handle *handle, int gen);
ALG_MATH_API alg_state de_free(de_handle *handle);

#endif // !__DE_H
