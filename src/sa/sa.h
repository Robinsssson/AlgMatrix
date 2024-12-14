#ifndef __SA_H
#define __SA_H

#include "../basic_opti.h"

typedef struct {
    optim_handle optim;
    alg_vector *current_solution;
    alg_vector *best_solution, *new_solution;
    double current_energy, best_energy;
    double temperature, cooling_rate;
} sa_handle;

ALG_MATH_API sa_handle *sa_init(optim_handle optim, double temperature, double cooling_rate);
ALG_MATH_API alg_state sa_fresh(sa_handle *handle, int gen);
ALG_MATH_API alg_state sa_free(sa_handle *handle);

#endif // !__SA_H
