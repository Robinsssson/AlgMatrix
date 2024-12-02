#ifndef __ABC_H
#define __ABC_H

#include "alg_inc.h"
#include <algmath.h>

typedef struct {
    int food_number;
    int dim;
    double lower_bound, upper_bound;
    int limit;
    alg_matrix *food_matrix;
    double (*evaluate)(alg_vector *food);
    alg_matrix *employed_bees, *onlooker_bees;
    alg_vector *fitness;
} abc_handle;

typedef double (*evaluate_function)(alg_vector *food);

abc_handle *abc_init(int food_number, int dimension, double lower_bound, double upper_bound, int limit,
                     evaluate_function evaluate);
alg_state abc_fresh(abc_handle *handle);
alg_state abc_free(abc_handle *handle);
int abc_get_best_solution(abc_handle *handle);
#endif
