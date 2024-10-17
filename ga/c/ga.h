#ifndef __GA_H
#define __GA_H

#include "../../algmath/algmath.h"
typedef double (*ga_aim_function)(alg_vector *);
typedef struct {
    alg_matrix *population;
    alg_vector *fitness;
    int pop_size, args_number;
    double mutation_rate, crossover_rate, var_max, var_min;
    ga_aim_function function;
} ga_handle;

ga_handle *ga_init(int pop_size, double mutation_rate, double crossover_rate, ga_aim_function function, int args_number, double var_max, double var_min);
void ga_fresh(ga_handle *ga);
void ga_free(ga_handle *ga);

#endif
