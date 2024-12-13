#ifndef __BASIC_OPTI_H
#define __BASIC_OPTI_H
#include "algmath.h"

/*
 * function pointer for optimization
 */
typedef double (*optimization)(alg_vector *);
typedef struct {
    int dim;
    alg_vector *l_range, *r_range;
    optimization function;
    alg_vector *best_solution;
    double bast_value;
} optim_handle;

alg_state optim_init(optim_handle *handle, int dim, optimization function, double l_range[], double r_range[]);
alg_state optim_free(optim_handle *handle);
alg_state optim_fresh(optim_handle *handle, alg_matrix *population, alg_vector *fitness);
alg_state optim_print(optim_handle *handle);
#endif //!__BASIC_OPTI_H
