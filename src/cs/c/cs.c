#include "cs.h"
#include "alg_inc.h"
#include "math/alg_math.h"
#include "matrix/alg_matrix.h"
#include "memalloc/alg_memalloc.h"
#include "random/alg_random.h"
#include "vector/alg_vector.h"
#include <math.h>

static double levy_flight(double lambda) {
    double sigma1 = pow(alg_math_gamma(1 + lambda) * sin(ALG_PI * lambda / 2) / alg_math_gamma((1 + lambda) / 2)
                            * pow(2, (lambda - 1) / 2),
                        1 / lambda),
           sigma2 = 1, u, v, epsilon = 1e-8;
    u = alg_random_normal(0, sigma1);
    do {
        v = alg_random_normal(0, sigma2);
    } while (fabs(v) < epsilon); // 避免 v 过小导致除以零
    double step = u / pow(fabs(v), 1 / lambda);
    return step;
}

static int heaviside(double x) {
    return x > 0.0 ? 1 : 0;
}

cs_handle *cs_init(optim_handle optim, int pop_size, double step_size, double pa, double lambda) {
    cs_handle *handle = ALG_MALLOC(sizeof(cs_handle));
    if (handle == NULL) {
        fprintf(stderr, "Memory allocation for cs_handle failed\n");
        return NULL;
    }
    handle->optim = optim;
    handle->pop_size = pop_size;
    handle->step_size = step_size;
    handle->pa = pa;
    handle->lambda = lambda;

    handle->fitness = alg_vector_create(pop_size, 0.0);
    if (!handle->fitness) {
        fprintf(stderr, "Vector creation failed\n");
        ALG_FREE(handle);
        return NULL;
    }

    handle->population = alg_matrix_create(pop_size, optim.dim);
    if (!handle->population) {
        fprintf(stderr, "Matrix creation failed\n");
        alg_vector_free(handle->fitness);
        ALG_FREE(handle);
        return NULL;
    }

    alg_matrix_fill_random_vecs(handle->population, optim.l_range, optim.r_range, SET_ROW);
    optim_fresh(&handle->optim, handle->population, handle->fitness);
    return handle;
}

alg_state cs_fresh(cs_handle *handle, int gen) {
    alg_vector *new_solve = alg_vector_create(handle->optim.dim, 0.0),
               *cache_solve = alg_vector_create(handle->optim.dim, 0.0),
               *m_solve = alg_vector_create(handle->optim.dim, 0.0),
               *n_solve = alg_vector_create(handle->optim.dim, 0.0);
    double cache_fitval, new_fitval, rand_u;
    int diff_index[2];
    int m, n;
    for (int __iter = 0; __iter < gen; __iter++) {
        // 1. create new solution from levy flights
        for (int i = 0; i < handle->pop_size; i++) {
            for (int j = 0; j < handle->population->col; j++) {
                cache_solve->vector[j] = *alg_matrix_get_pos_val(handle->population, i, j);
                new_solve->vector[j] = cache_solve->vector[j] + handle->step_size * levy_flight(handle->lambda);
            }
            alg_vector_claim_vecs(new_solve, handle->optim.l_range, handle->optim.r_range);
            cache_fitval = handle->optim.function(cache_solve);
            new_fitval = handle->optim.function(new_solve);
            if (new_fitval < cache_fitval) {
                alg_matrix_set_row(handle->population, i, new_solve);
                handle->fitness->vector[i] = new_fitval;
                for (int j = 0; j < handle->population->col; j++) {
                    cache_solve->vector[j] = new_solve->vector[j];
                }
            }

            do {
                alg_random_sample_unique(0, handle->pop_size, 2, diff_index);
            } while (diff_index[0] != i && diff_index[1] != i);

            m = diff_index[0];
            n = diff_index[1];

            for (int j = 0; j < handle->optim.dim; j++) {
                m_solve->vector[j] = *alg_matrix_get_pos_val(handle->population, m, j);
                n_solve->vector[j] = *alg_matrix_get_pos_val(handle->population, n, j);
            }

            rand_u = alg_random_float64(0, 1);

            for (int j = 0; j < handle->optim.dim; j++) {
                if (heaviside(handle->pa - alg_random_float64(0, 1)))
                    new_solve->vector[j] = cache_solve->vector[j] + rand_u * (m_solve->vector[j] - n_solve->vector[j]);
            }
            alg_vector_claim_vecs(new_solve, handle->optim.l_range, handle->optim.r_range);
            
            cache_fitval = handle->optim.function(cache_solve);
            new_fitval = handle->optim.function(new_solve);

            if (new_fitval < cache_fitval) {
                alg_matrix_set_row(handle->population, i, new_solve);
                handle->fitness->vector[i] = new_fitval;
            }

            optim_fresh(&handle->optim, handle->population, handle->fitness);
        }
    }
    alg_vector_free(new_solve);
    alg_vector_free(cache_solve);
    return ALG_OK;
}

alg_state cs_free(cs_handle *handle) {
    optim_free(&handle->optim);
    alg_matrix_free(handle->population);
    alg_vector_free(handle->fitness);
    ALG_FREE(handle);
    return ALG_OK;
}
