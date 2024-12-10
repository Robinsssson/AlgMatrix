#ifndef __CSO_H
#define __CSO_H

#include "../../basic_opti.h"
#include "alg_inc.h"

typedef struct {
    int mf, rf, cf;
    int dim;
    int number;
    double llim, rlim;
    alg_matrix *population;
    alg_vector *fitness;
    optimization function;
    int *sorted_index;
} cso_handle;

cso_handle *cso_init(int dim, int n, double llim, double rlim, int mf, int rf, optimization function);
alg_state cso_fresh(cso_handle *handle);
alg_state cso_free(cso_handle* handle);

#endif
