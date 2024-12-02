#include "aco.h"
#include "algmath.h"
#include <math.h>
// Solve TSP Question

aco_handle *aco_init(int number, alg_matrix *city_coords, double alpha, double beta, double rho) {
    aco_handle *aco = ALG_MALLOC(sizeof(aco_handle));
    if (aco == NULL) {
        ERROR("ERROR IN INIT ACO HANDLE");
        return NULL;
    }
    aco->alpha = alpha;
    aco->best_length = INFINITY;
    aco->best_path = alg_vector_create(city_coords->col, -1);
    if (aco->best_path == NULL) {
        ERROR("ERROR IN INIT BEST_PATH HANDLE");
        ALG_FREE(aco);
    }
    aco->Q = 100;
    aco->beta = beta;
    aco->rho = rho;
    aco->number = number;
    if (city_coords == NULL) {
        ERROR("INPUT VAL CITY COORDS DID NOT LEAGE, (NUMBER, DIM)");
        alg_vector_free(aco->best_path);
        ALG_FREE(aco);
    }
    aco->city_coords = alg_matrix_copy(city_coords);
    aco->pheromone = alg_matrix_create(number, number);
}

int aco_fresh(aco_handle *aco) {}

int aco_free(aco_handle *handle);