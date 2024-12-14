#ifndef __PSO__H__
#define __PSO__H__

#include "../../basic_opti.h"

typedef struct {
    optim_handle optim;
    double w;
    double c1, c2;
    int pop_size;
    int g_best_index;
    double *p_best_fitness, g_best_fitness;
    alg_matrix *position;
    alg_matrix *vec;
    alg_matrix *p_best, *g_best;
    alg_vector *fitness;
} pso_handle;

ALG_MATH_API pso_handle *pso_init(optim_handle optim, int pop_size, double w, double c1, double c2);
ALG_MATH_API alg_state pso_fresh(pso_handle *handle, int gen);
ALG_MATH_API alg_state pso_free(pso_handle *handle);

#endif //!__PSO__H__
