#include "aco.h"
#include "alg_inc.h"
#include "algmath.h"
#include "matrix/alg_matrix.h"
#include "memalloc/alg_memalloc.h"
#include "random/alg_random.h"
#include "vector/alg_vector.h"
#include <math.h>
#include <string.h>

// Solve TSP Question

aco_handle *aco_init(int number, alg_matrix *city_coords, double alpha, double beta, double rho) {
    aco_handle *aco = ALG_MALLOC(sizeof(aco_handle));
    if (aco == NULL) {
        ERROR("ERROR IN INIT ACO HANDLE");
        return NULL;
    }
    aco->alpha = alpha;
    aco->best_length = INFINITY;
    aco->best_path = alg_vector_create(city_coords->row, -1);
    if (aco->best_path == NULL) {
        ERROR("ERROR IN INIT BEST_PATH HANDLE");
        ALG_FREE(aco);
        return NULL;
    }
    aco->Q = 100;
    aco->beta = beta;
    aco->rho = rho;
    aco->number = number;
    if (city_coords == NULL) {
        ERROR("INPUT VAL CITY COORDS DID NOT LEAGE, (NUMBER, DIM)");
        alg_vector_free(aco->best_path);
        ALG_FREE(aco);
        return NULL;
    }
    aco->city_coords = alg_matrix_copy(city_coords);
    aco->pheromone = alg_matrix_create_val(number, number, 1);
    aco->dist = alg_matrix_create(aco->city_coords->row, aco->city_coords->row);
    int n = aco->city_coords->row;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double x1 = *alg_matrix_get_pos_val(city_coords, i, 0);
            double x2 = *alg_matrix_get_pos_val(city_coords, j, 0);
            double y1 = *alg_matrix_get_pos_val(city_coords, i, 1);
            double y2 = *alg_matrix_get_pos_val(city_coords, j, 1);
            alg_matrix_set_val(aco->dist, i, j, sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
        }
    }
    return aco;
}

static alg_val_type alg_vector_sum(const alg_vector *vec) {
    alg_val_type val = 0.0;
    for (int i = 0; i < vec->size; i++)
        val += vec->vector[i];
    return val;
}

static int roulette_wheel_selection(alg_vector *probabilities) {
    alg_vector *copy = alg_vector_create_like(probabilities);
    if (!copy) {
        ERROR("Memory allocation failed for probabilities copy");
        return -1;
    }

    for (int index = 1; index < copy->size; index++) {
        copy->vector[index] += copy->vector[index - 1];
    }
    double rand_val = alg_random_float64(0, 1);
    for (int index = 0; index < copy->size; index++) {
        if (copy->vector[index] > rand_val) {
            alg_vector_free(copy);
            return index;
        }
    }
    alg_vector_free(copy);
    return -1; // In case of failure
}

alg_state aco_fresh(aco_handle *handle) {
    int N = handle->city_coords->row;
    int path[handle->number][N];

    for (int i = 0; i < handle->number; i++) {
        for (int j = 0; j < N; j++) {
            path[i][j] = 0;
        }
    }
    alg_vector *path_lengths = alg_vector_create(handle->number, 0);
    if (path_lengths == NULL) {
        ERROR("ACO CREATE path_lengths IS ERROR");
        return ALG_ERROR;
    }

    for (int iter = 0; iter < handle->number; iter++) {
        int visited[N];
        memset(visited, 0, sizeof(visited));
        int city = alg_random_int(0, N); // Fix random generation
        visited[city] = 1;
        path[iter][0] = city;

        for (int step = 1; step < N; step++) {
            alg_vector *probabilities = alg_vector_create(N, 0.0);
            if (!probabilities) {
                ERROR("ACO CREATE probabilities IS ERROR");
                return ALG_ERROR;
            }

            for (int iter_N = 0; iter_N < N; iter_N++) {
                if (visited[iter_N])
                    continue;
                double val_of_pheromone = *alg_matrix_get_pos_val(handle->pheromone, city, iter_N);
                double dist_val = 1 / *alg_matrix_get_pos_val(handle->dist, city, iter_N);
                alg_vector_set_val(probabilities, iter_N,
                                   pow(val_of_pheromone, handle->alpha) * pow(dist_val, handle->beta));
            }
            double sum_val = alg_vector_sum(probabilities);
            for (int _iter_probabilities = 0; _iter_probabilities < probabilities->size; _iter_probabilities++) {
                probabilities->vector[_iter_probabilities] /= sum_val;
            }
            int next_city = roulette_wheel_selection(probabilities);
            if (next_city == -1) {
                char *str_logging = alg_vector_print_str(probabilities);
                printf("%s%f\n", str_logging, sum_val);
                alg_free(str_logging);
                alg_vector_free(probabilities);
                ERROR("ACO CREATE roulette_wheel_selection IS ERROR");
                return ALG_ERROR; // Failure in selection
            }
            visited[next_city] = 1;
            path[iter][step] = next_city;
            city = next_city;
            alg_vector_free(probabilities);
        }

        double path_length = 0.0;
        for (int i = 0; i < N - 1; i++) {
            path_length += *alg_matrix_get_pos_val(handle->dist, path[iter][i], path[iter][i + 1]);
        }
        path_length += *alg_matrix_get_pos_val(handle->dist, path[iter][N - 1], path[iter][0]);
        alg_vector_set_val(path_lengths, iter, path_length);

        if (path_length < handle->best_length) {
            handle->best_length = path_length;
            for (int iter_N = 0; iter_N < N; iter_N++) {
                alg_vector_set_val(handle->best_path, iter_N, path[iter][iter_N]);
            }
        }
    }

    // Update pheromone
    for (int iter_pher = 0; iter_pher < handle->pheromone->col * handle->pheromone->row; iter_pher++) {
        handle->pheromone->mat[iter_pher] *= 1 - handle->rho;
    }

    for (int k = 0; k < handle->number; k++) {
        for (int i = 0; i < N - 1; i++) {
            double *mutval = alg_matrix_get_pos_mutval(handle->pheromone, path[k][i], path[k][i + 1]);
            *mutval = (*mutval + handle->Q) / path_lengths->vector[k];
        }
        double *mutval = alg_matrix_get_pos_mutval(handle->pheromone, path[k][N - 1], path[k][0]);
        *mutval = (*mutval + handle->Q) / path_lengths->vector[k];
    }
    alg_vector_free(path_lengths);
    return ALG_OK;
}

alg_state aco_free(aco_handle *handle) {
    alg_vector_free(handle->best_path);
    alg_matrix_free(handle->pheromone);
    alg_matrix_free(handle->city_coords);
    alg_matrix_free(handle->dist);
    ALG_FREE(handle);
    return ALG_OK;
}
