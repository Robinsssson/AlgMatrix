#include "cso.h"
#include "alg_inc.h"
#include "matrix/alg_matrix.h"
#include "memalloc/alg_memalloc.h"
#include "utils/alg_utils.h"
#include "vector/alg_vector.h"
#include <stdio.h>

static void fresh_fitness(cso_handle *handle) {
    alg_vector *tmp_vec = alg_vector_create(handle->dim, 0.0);
    for (int i = 0; i < handle->number; i++) {
        for (int j = 0; j < handle->dim; j++) {
            tmp_vec->vector[j] = *alg_matrix_get_pos_val(handle->population, i, j);
        }
        handle->fitness->vector[i] = handle->function(tmp_vec);
    }
    alg_vector_free(tmp_vec);
}

cso_handle *cso_init(int dim, int n, double llim, double rlim, int mf, int rf, optimization function) {
    cso_handle *handle = ALG_MALLOC(sizeof(cso_handle));
    handle->population = alg_matrix_create(n, dim);
    handle->fitness = alg_vector_create(n, 0.0);
    handle->cf = n - rf - mf;
    handle->mf = mf;
    handle->rf = rf;
    handle->dim = dim;
    handle->llim = llim;
    handle->rlim = rlim;
    handle->function = function;
    alg_matrix_fill_random(handle->population, llim, rlim);
    fresh_fitness(handle);
    handle->sorted_index = ALG_MALLOC(sizeof(int) * (size_t)n);
    alg_vector_sort_inplace(handle->fitness, handle->sorted_index, ALG_FALSE, alg_utils_greater);
    return handle;
}

alg_state cso_fresh(cso_handle *handle) {
    alg_vector *step = alg_vector_create(handle->dim, 0.0);

    for (int i = 0; i < handle->rf; i++) {
    }

    for (int i = handle->rf; i < handle->rf + handle->mf; i++) {
    }

    for (int i = handle->rf + handle->mf; i < handle->rf + handle->mf + handle->cf; i++) {
    }

    alg_vector_sort_inplace(handle->fitness, handle->sorted_index, ALG_FALSE, alg_utils_greater);
    alg_vector_free(step);
    return ALG_OK;
}

alg_state cso_free(cso_handle *handle) {
    ALG_FREE(handle);
    return ALG_OK;
}
