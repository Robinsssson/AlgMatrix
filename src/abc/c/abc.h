#ifndef __ABC_H
#define __ABC_H

#include "../../basic_opti.h"

typedef struct {
    int food_number;
    int dim;
    double lower_bound, upper_bound;
    int limit;
    alg_matrix *food_matrix;
    optimization function;
    alg_matrix *employed_bees, *onlooker_bees;
    alg_vector *fitness;
    int trial[];
} abc_handle;

abc_handle *abc_init(int food_number, int dimension, double lower_bound, double upper_bound, int limit,
                     optimization function);
alg_state abc_fresh(abc_handle *handle);
alg_state abc_free(abc_handle *handle);
int abc_get_best_solution(abc_handle *handle);
#endif
