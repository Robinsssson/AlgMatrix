#ifndef __EDA_H
#define __EDA_H

#include "alg_inc.h"
#include <algmath.h>

typedef double (*optimization)(alg_vector *);

typedef struct {
    int dim, population_size;
    alg_matrix *population;
    alg_vector *fitness;
    double llim, rlim;
    double elitism_rate, best_val;
    alg_vector *best_solve;
    optimization function;
} eda_handle;

eda_handle *eda_init(int dim, int population_size, double llim, double rlim, double elitism_rate,
                     optimization function);
alg_state eda_fresh(eda_handle *handle);
alg_state eda_free(eda_handle *handle);

#endif
