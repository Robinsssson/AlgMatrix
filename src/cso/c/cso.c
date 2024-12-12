#include "cso.h"
#include "matrix/alg_matrix.h"
#include <math.h>

static alg_state fresh_fitness(cso_handle *handle) {
    alg_vector *vec = alg_vector_create(handle->dim, 0.0);
    for (int pop = 0; pop < handle->pop_size; pop++) {
        for (int i = 0; i < handle->dim; i++) {
            vec->vector[i] = *alg_matrix_get_pos_val(handle->position, pop, i);
        }
        handle->fitness->vector[pop] = handle->function(vec);
    }
    alg_vector_free(vec);
    return ALG_OK;
}

cso_handle *cso_init(int pop_size, int dim, double llim, double rlim, optimization func) {
    cso_handle *handle = ALG_MALLOC(sizeof(cso_handle));
    handle->pop_size = pop_size;
    handle->dim = dim;
    handle->llim = llim;
    handle->rlim = rlim;
    handle->function = func;

    handle->position = alg_matrix_create(pop_size, dim);
    handle->velocity = alg_matrix_create(pop_size, dim);
    handle->fitness = alg_vector_create(pop_size, INFINITY);
    handle->global_best_position = alg_vector_create(dim, 0.0);
    handle->global_best_fitness = INFINITY;

    alg_matrix_fill_random(handle->position, llim, rlim);
    fresh_fitness(handle);
    return handle;
}

alg_state cso_fresh(cso_handle *handle) {
    for (int i = 0; i < handle->pop_size; i++) {
        int leader = alg_random_int(0, handle->pop_size);
        int follower = alg_random_int(0, handle->pop_size);
        for (int d = 0; d < handle->dim; d++) {
            double *pos = alg_matrix_get_pos_val(handle->position, i, d);
            double *vel = alg_matrix_get_pos_val(handle->velocity, i, d);
            *vel = 2 * alg_random_float64(0, 1) * (*alg_matrix_get_pos_val(handle->position, leader, d) - *pos)
                   + 0.5 * alg_random_float64(0, 1) * (*alg_matrix_get_pos_val(handle->position, follower, d) - *pos);
            *pos += *vel;

            if (*pos < handle->llim)
                *pos = handle->llim;
            if (*pos > handle->rlim)
                *pos = handle->rlim;
        }
        double fitness = handle->func(handle->position->matrix[i]);
        if (fitness < *alg_vector_get_val(handle->fitness, i)) {
            alg_vector_set_val(handle->fitness, i, fitness);
        }
        if (fitness < handle->global_best_fitness) {
            handle->global_best_fitness = fitness;
            alg_vector_copy(handle->global_best_position, handle->position->matrix[i]);
        }
    }
    return ALG_OK;
}

alg_state cso_free(cso_handle *handle) {
    alg_matrix_free(handle->position);
    alg_matrix_free(handle->velocity);
    alg_vector_free(handle->fitness);
    alg_vector_free(handle->global_best_position);
    ALG_FREE(handle);
    return ALG_OK;
}
