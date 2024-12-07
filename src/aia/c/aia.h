#ifndef __AIA_H
#define __AIA_H
#include <algmath.h>
typedef double (*optimization)(alg_vector *);
typedef struct {
    int clone_factor, dimension, num_antibodies;
    double llim, rlim, best_fitness, mutation_rate;
    alg_matrix *population;
    alg_vector *fitness, *best_solve;
    optimization function;
} aia_handle;

aia_handle *aia_init(int num_antibodies, int clone_factor, int dimension, double mutation_rate, double llim,
                     double rlim, optimization function);
alg_state aia_fresh(aia_handle *handle);
void aia_free(aia_handle *handle);
#endif
