#ifndef __DE_H
#define __DE_H

#include "../../basic_opti.h"
#include "alg_inc.h"

typedef struct {
    int pop_size;
    int dim;
    double rlim, llim;
    double f;
    double cr;
    optimization function;
    alg_matrix *population;
    alg_vector *fitness;
} de_handle;

de_handle *de_init(int pop_size, int dim, double llim, double rlim, optimization function, double f, double cr);
alg_state de_fresh(de_handle *handle);
alg_state de_free(de_handle *handle);

#endif // !__DE_H
