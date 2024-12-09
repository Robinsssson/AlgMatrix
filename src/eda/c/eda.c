#include "eda.h"
#include "alg_inc.h"
#include "matrix/alg_matrix.h"
#include "memalloc/alg_memalloc.h"
#include "random/alg_random.h"
#include "utils/alg_utils.h"
#include "vector/alg_vector.h"
#include <math.h>

eda_handle *eda_init(int dim, int population_size, double llim, double rlim, double elitism_rate,
                     optimization function) {
    eda_handle *handle = ALG_MALLOC(sizeof(eda_handle));
    if (handle == NULL) {
        ERROR("CREATE EDA HANDLE ERROR");
        return NULL;
    }

    handle->population = alg_matrix_create(population_size, dim);
    if (handle == NULL) {
        ERROR("CREATE EDA population HANDLE ERROR");
        ALG_FREE(handle);
        return NULL;
    }

    handle->fitness = alg_vector_create(population_size, INFINITY);
    if (handle == NULL) {
        ERROR("CREATE EDA fitness HANDLE ERROR");
        alg_matrix_free(handle->population);
        ALG_FREE(handle);
        return NULL;
    }

    handle->best_solve = alg_vector_create(dim, INFINITY);
    if (handle == NULL) {
        ERROR("CREATE EDA best_solve HANDLE ERROR");
        alg_vector_free(handle->fitness);
        alg_matrix_free(handle->population);
        ALG_FREE(handle);
        return NULL;
    }

    alg_matrix_fill_random(handle->population, llim, rlim);
    handle->dim = dim;
    handle->population_size = population_size;
    handle->llim = llim;
    handle->rlim = rlim;
    handle->elitism_rate = elitism_rate;
    handle->function = function;
    return handle;
}

static alg_state fresh_fitness(eda_handle *handle) {
    alg_vector *vec = alg_vector_create(handle->dim, 0.0);
    for (int pop = 0; pop < handle->population_size; pop++) {
        for (int i = 0; i < handle->dim; i++) {
            vec->vector[i] = *alg_matrix_get_pos_val(handle->population, pop, i);
        }
        handle->fitness->vector[pop] = handle->function(vec);
    }
    alg_vector_free(vec);
    return ALG_OK;
}

alg_state eda_fresh(eda_handle *handle) {
    int sorted_index[handle->population_size];
    fresh_fitness(handle);
    int index = alg_vector_compare_val(handle->fitness, alg_utils_greater);
    for (int i = 0; i < handle->dim; i++)
        handle->best_solve->vector[i] = *alg_matrix_get_pos_val(handle->population, index, i);
    handle->best_val = handle->fitness->vector[index];
    alg_vector *sorted_fitness = alg_vector_create_like(handle->fitness);
    alg_vector_sort_copy(handle->fitness, sorted_fitness, sorted_index, alg_utils_greater);
    int elite_count = (int)round(handle->elitism_rate * handle->population_size);
    alg_matrix *elites = alg_matrix_create(elite_count, handle->dim);
    for (int i = 0; i < elite_count; i++) {
        for (int j = 0; j < handle->dim; j++) {
            alg_matrix_set_val(elites, i, j, *alg_matrix_get_pos_val(handle->population, sorted_index[i], j));
        }
    }

    double mean[handle->dim];

    for (int i = 0; i < handle->dim; i++) {
        mean[i] = 0.0;
        for (int j = 0; j < elite_count; j++) {
            mean[i] += *alg_matrix_get_pos_val(elites, j, i);
        }
        mean[i] /= elite_count;
    }
    double std[handle->dim];

    for (int i = 0; i < handle->dim; i++) {
        std[i] = 0.0;
        for (int j = 0; j < elite_count; j++) {
            std[i] += pow(*alg_matrix_get_pos_val(elites, j, i) - mean[i], 2);
        }
        std[i] /= elite_count;
        std[i] = sqrt(std[i]);
    }
    for (int i = 0; i < handle->population_size; i++) {
        for (int j = 0; j < handle->dim; j++) {
            alg_matrix_set_val(handle->population, i, j, alg_random_normal(mean[j], std[j]));
        }
    }

    alg_matrix_clamp(handle->population, handle->llim, handle->rlim);

    alg_matrix_free(elites);
    alg_vector_free(sorted_fitness);
    return ALG_OK;
}
alg_state eda_free(eda_handle *handle) {
    alg_matrix_free(handle->population);
    alg_vector_free(handle->fitness);
    alg_vector_free(handle->best_solve);
    ALG_FREE(handle);
    return ALG_OK;
}
