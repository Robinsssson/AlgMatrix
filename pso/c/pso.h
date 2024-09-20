#ifndef __PSO__H__
#define __PSO__H__
#include "../../algmath/algmath.h"
typedef double (*pso_aim_function)(alg_vector *);
typedef enum {
    PSO_USE_MAX,
    PSO_USE_MIN,
} pso_type;
typedef struct {
    double w;
    double c1, c2;
    double r1, r2;
    int dim, number;
    int g_best_index;
    double *p_best_fitness, g_best_fitness;
    alg_matrix *position;
    alg_matrix *vec;
    alg_matrix *p_best, *g_best;
    pso_aim_function func;
    pso_type type;
} pso_handle;

pso_handle *pso_init(int number, int dim, double w, double c1, double c2, pso_aim_function function, pso_type type);
void pso_fresh(pso_handle *pso_structure, double min_vec, double max_vec);
pso_handle *pso_free(pso_handle *pso_structure);

#endif //!__PSO__H__
