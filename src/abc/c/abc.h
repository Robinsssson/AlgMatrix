#ifndef __ABC_H
#define __ABC_H

#include "../../basic_opti.h"

typedef struct {
    optim_handle optim;
    int pop_size;
    
} abc_handle;

abc_handle *abc_init(int food_number, int dimension, double lower_bound, double upper_bound, int limit,
                     optimization function);
alg_state abc_fresh(abc_handle *handle);
alg_state abc_free(abc_handle *handle);
int abc_get_best_solution(abc_handle *handle);
#endif
