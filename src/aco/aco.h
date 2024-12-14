#ifndef __ACO_H
#define __ACO_H

#include "../basic_opti.h"

typedef struct {
    alg_matrix *city_coords;
    int number;
    double alpha, beta, Q, rho;
    alg_vector *best_path;
    double best_length;
    alg_matrix *pheromone, *dist;
} aco_handle;
ALG_MATH_API aco_handle *aco_init(int number, alg_matrix *city_coords, double alpha, double beta, double rho);
ALG_MATH_API alg_state aco_fresh(aco_handle *handle);
ALG_MATH_API alg_state aco_free(aco_handle *handle);
#endif // !__ACO_H
