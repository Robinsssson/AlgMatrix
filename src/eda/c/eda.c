#include "eda.h"
#include "alg_inc.h"
#include "matrix/alg_matrix.h"
#include "memalloc/alg_memalloc.h"
#include "random/alg_random.h"
#include "utils/alg_utils.h"
#include "vector/alg_vector.h"
#include <math.h>

eda_handle *eda_init(optim_handle optim, int pop_size, double elitism_rate) {
    eda_handle *handle = ALG_MALLOC(sizeof(eda_handle));
    if (handle == NULL) {
        ERROR("CREATE EDA HANDLE ERROR");
        return NULL;
    }

    handle->population = alg_matrix_create(pop_size, optim.dim);
    if (handle->population == NULL) {
        ERROR("CREATE EDA population HANDLE ERROR");
        ALG_FREE(handle);
        return NULL;
    }

    handle->fitness = alg_vector_create(pop_size, INFINITY);
    if (handle->fitness == NULL) {
        ERROR("CREATE EDA fitness HANDLE ERROR");
        alg_matrix_free(handle->population);
        ALG_FREE(handle);
        return NULL;
    }

    alg_matrix_fill_random_vecs(handle->population, optim.l_range, optim.r_range, SET_ROW);
    handle->pop_size = pop_size;
    handle->elitism_rate = elitism_rate;
    handle->optim = optim;

    optim_fresh(&handle->optim, handle->population, handle->fitness);
    return handle;
}

alg_state eda_fresh(eda_handle *handle, int gen) {
    double mean[handle->optim.dim];
    int sorted_index[handle->pop_size];
    double std[handle->optim.dim];
    for (int __iter = 0; __iter < gen; __iter++) {

        alg_vector *sorted_fitness = alg_vector_create_like(handle->fitness);
        alg_vector_sort_copy(sorted_fitness, handle->fitness, sorted_index, alg_utils_greater);
        int elite_count = (int)round(handle->elitism_rate * handle->pop_size);
        alg_matrix *elites = alg_matrix_create(elite_count, handle->optim.dim);
        for (int i = 0; i < elite_count; i++) {
            for (int j = 0; j < handle->optim.dim; j++) {
                alg_matrix_set_val(elites, i, j, *alg_matrix_get_pos_val(handle->population, sorted_index[i], j));
            }
        }

        for (int i = 0; i < handle->optim.dim; i++) {
            mean[i] = 0.0;
            for (int j = 0; j < elite_count; j++) {
                mean[i] += *alg_matrix_get_pos_val(elites, j, i);
            }
            mean[i] /= elite_count;
        }

        for (int i = 0; i < handle->optim.dim; i++) {
            std[i] = 0.0;
            for (int j = 0; j < elite_count; j++) {
                std[i] += pow(*alg_matrix_get_pos_val(elites, j, i) - mean[i], 2);
            }
            std[i] /= elite_count;
            std[i] = sqrt(std[i]);
        }
        
        for (int i = 0; i < handle->pop_size; i++) {
            for (int j = 0; j < handle->optim.dim; j++) {
                alg_matrix_set_val(handle->population, i, j, alg_random_normal(mean[j], std[j]));
            }
        }

        alg_matrix_fill_random_vecs(handle->population, handle->optim.l_range, handle->optim.r_range, SET_ROW);

        optim_fresh(&handle->optim, handle->population, handle->fitness);
        alg_matrix_free(elites);
        alg_vector_free(sorted_fitness);
    }

    return ALG_OK;
}

alg_state eda_free(eda_handle *handle) {
    alg_matrix_free(handle->population);
    alg_vector_free(handle->fitness);
    ALG_FREE(handle);
    return ALG_OK;
}
