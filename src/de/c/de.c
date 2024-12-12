#include "de.h"
#include "alg_inc.h"
#include "matrix/alg_matrix.h"
#include "memalloc/alg_memalloc.h"
#include "random/alg_random.h"
#include "vector/alg_vector.h"
#include <math.h>
#include <stdio.h>

static alg_boolean check_if(int *idx, int len, int index) {
    for (int i = 0; i < len; i++)
        if (idx[i] == index)
            return ALG_TRUE;
    return ALG_FALSE;
}

static alg_state get_from_index(const alg_matrix *mat, double *arr, int length, int index) {
    if (length == mat->col) {
        for (int i = 0; i < length; i++) {
            arr[i] = *alg_matrix_get_pos_val(mat, index, i);
        }
    } else if (length == mat->row) {
        for (int i = 0; i < length; i++) {
            arr[i] = *alg_matrix_get_pos_val(mat, i, index);
        }
    } else
        return ALG_ERROR;
    return ALG_OK;
}

static alg_state fresh_fitness(de_handle *handle) {
    alg_vector *vec = alg_vector_create(handle->dim, 0.0);
    for (int pop = 0; pop < handle->pop_size; pop++) {
        for (int i = 0; i < handle->dim; i++) {
            vec->vector[i] = *alg_matrix_get_pos_val(handle->population, pop, i);
        }
        handle->fitness->vector[pop] = handle->function(vec);
    }
    alg_vector_free(vec);
    return ALG_OK;
}

de_handle *de_init(int pop_size, int dim, double llim, double rlim, optimization function, double f, double cr) {
    de_handle *handle = ALG_MALLOC(sizeof(de_handle));
    handle->fitness = alg_vector_create(pop_size, INFINITY);
    handle->population = alg_matrix_create(pop_size, dim);
    alg_matrix_fill_random(handle->population, llim, rlim);
    handle->pop_size = pop_size;
    handle->dim = dim;
    handle->llim = llim;
    handle->rlim = rlim;
    handle->function = function;
    handle->f = f;
    handle->cr = cr;
    fresh_fitness(handle);
    return handle;
}

alg_state de_fresh(de_handle *handle) {
    int idx[3];
    int j_rand;
    alg_vector *u;
    double u_fitness;
    double x1[handle->dim], x2[handle->dim], x3[handle->dim], v[handle->dim];
    for (int i = 0; i < handle->pop_size; i++) {
        do {
            alg_random_sample_unique(0, (int)handle->pop_size, 3, idx);
        } while (check_if(idx, 3, i) == ALG_TRUE);

        if (get_from_index(handle->population, x1, handle->dim, idx[0]) != ALG_OK
            || get_from_index(handle->population, x2, handle->dim, idx[1]) != ALG_OK
            || get_from_index(handle->population, x3, handle->dim, idx[2]) != ALG_OK) {
            return ALG_ERROR;
        }

        for (int i_dim = 0; i_dim < handle->dim; i_dim++)
            v[i_dim] = x1[i_dim] + handle->f * (x2[i_dim] - x3[i_dim]);

        // 交叉操作
        u = alg_vector_from_matrix_row(handle->population, i);
        j_rand = alg_random_int(0, handle->dim);
        for (int j = 0; j < handle->dim; j++) {
            if (alg_random_float64(0, 1) <= handle->cr || j == j_rand)
                alg_vector_set_val(u, j, v[j]);
        }
        alg_vector_claim(u, handle->llim, handle->rlim);
        // 选择操作
        u_fitness = handle->function(u);
        if (u_fitness < handle->fitness->vector[i]) {
            alg_matrix_set_row(handle->population, i, u);
            handle->fitness->vector[i] = u_fitness;
        }
        alg_vector_free(u);
    }
    return ALG_OK;
}

alg_state de_free(de_handle *handle) {
    alg_vector_free(handle->fitness);
    alg_matrix_free(handle->population);
    ALG_FREE(handle);
    return ALG_OK;
}
