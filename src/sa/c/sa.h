#ifndef __SA_H
#define __SA_H

#include "../../basic_opti.h"

typedef struct {
    int dim;
    alg_vector *current_solution;
    alg_vector *best_solution, *new_solution;
    double current_energy, best_energy;
    double llim, rlim;
    double temperature, cooling_rate;
    optimization function;
} sa_handle;

sa_handle *sa_init(int dim, double temperature, double cooling_rate, double llim, double rlim, optimization function);
alg_state sa_fresh(sa_handle *handle);
alg_state sa_free(sa_handle *handle);

#endif // !__SA_H
