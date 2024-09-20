#ifndef __PSO__H__
#define __PSO__H__
#include "../../c_lib/alg_lib.h"
typedef double (*pso_aim_function)(alg_vector *);
typedef struct {
    double w;
    double c1, c2;
    double r1, r2;
    int dim, number;
    alg_matrix *position;
    alg_matrix *vec;
    alg_matrix *p_best, *g_best;
    pso_aim_function func;
} pso_handle;

pso_handle *pso_init(int number, int dim, double w, double c1, double c2,
                     pso_aim_function function);
void pso_fresh(pso_handle *pso_structure);
pso_handle *pso_free(pso_handle *pso_structure);

#endif //!__PSO__H__
