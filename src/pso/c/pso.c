#include "pso.h"
#include "alg_inc.h"
#include "algmath.h"
#include "matrix/alg_matrix.h"
#include "vector/alg_vector.h"
#include <corecrt.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Helper function to ALG_FREE PSO resources
static void pso_free_internal(pso_handle *handle) {
    alg_matrix_free(handle->g_best);
    alg_matrix_free(handle->p_best);
    alg_matrix_free(handle->position);
    alg_matrix_free(handle->vec);
    ALG_FREE(handle->p_best_fitness);
}

pso_handle *pso_init(optim_handle optim, int pop_size, double w, double c1, double c2) {
    pso_handle *handle = ALG_MALLOC(sizeof(pso_handle));
    if (!handle)
        return NULL;

    handle->vec = alg_matrix_create(pop_size, optim.dim);
    handle->position = alg_matrix_create(pop_size, optim.dim);
    handle->p_best = alg_matrix_copy(handle->position);
    handle->g_best = alg_matrix_create(pop_size, optim.dim); // Global best is usually a single particle
    handle->fitness = alg_vector_create(pop_size, 0.0);
    if (!handle->vec || !handle->position || !handle->p_best || !handle->g_best) {
        pso_free_internal(handle);
        return NULL;
    }

    handle->p_best_fitness = ALG_MALLOC((size_t)pop_size * sizeof(double));
    if (!handle->p_best_fitness) {
        pso_free_internal(handle);
        return NULL;
    }
    handle->optim = optim;
    handle->g_best_index = 0;
    handle->g_best_fitness = INFINITY;
    handle->c1 = c1;
    handle->c2 = c2;
    handle->pop_size = pop_size;
    handle->w = w;
    alg_matrix_fill_random_vecs(handle->vec, handle->optim.l_range, handle->optim.r_range, SET_ROW);
    alg_matrix_fill_random(handle->vec, -1, 1);
    optim_fresh(&handle->optim, handle->position, handle->fitness);
    return handle;
}

// Helper function to calculate fitness for a particle
static double pso_calculate_fitness(pso_handle *handle, int index) {
    alg_vector *vec = alg_vector_from_matrix_row(handle->position, index);
    double fitness = handle->optim.function(vec);
    alg_vector_free(vec);
    return fitness;
}

alg_state pso_fresh(pso_handle *handle, int gen) {
    double r1, r2;
    alg_vector *current_position = alg_vector_create(handle->optim.dim, 0.0);
    alg_vector *best_position = alg_vector_create(handle->optim.dim, 0.0);
    for (int __iter = 0; __iter < gen; __iter++) {
        double best_val = INFINITY;
        int best_idx = 0;

        // 遍历每个粒子
        for (int i = 0; i < handle->pop_size; i++) {
            double fitness = pso_calculate_fitness(handle, i);

            // 更新个人最佳位置和适应度
            if (fitness < handle->p_best_fitness[i]) {
                // 更新个人最佳适应度
                handle->p_best_fitness[i] = fitness;
                // 更新个人最佳位置
                alg_matrix_get_row(handle->position, current_position, i);
                alg_matrix_set_row(handle->p_best, i, current_position);
            }

            // 更新全局最佳
            if (fitness < best_val) {
                best_val = fitness;
                best_idx = i;
            }
        }

        // 更新全局最佳位置和适应度
        if (best_idx != handle->g_best_index) {
            alg_matrix_get_row(handle->position, best_position, best_idx);
            for (int i = 0; i < handle->pop_size; i++)
                alg_matrix_set_row(handle->g_best, i, best_position); // g_best 是单个粒子的位置
            handle->g_best_fitness = best_val;
            handle->g_best_index = best_idx;
        }

        // 更新粒子速度和位置
        alg_matrix_dot_number_inplace(handle->vec, handle->w);

        alg_matrix *sub1 = alg_matrix_subtraction(handle->p_best, handle->position);
        alg_matrix *sub2 = alg_matrix_subtraction(handle->g_best, handle->position);

        r1 = alg_random_float64(0, 1);
        r2 = alg_random_float64(0, 1);

        alg_matrix_dot_number_inplace(sub1, r1 * handle->c1);
        alg_matrix_dot_number_inplace(sub2, r2 * handle->c2);

        alg_matrix_add_inplace(handle->vec, sub1);
        alg_matrix_add_inplace(handle->vec, sub2);
        alg_matrix_clamp(handle->vec, -1, 1);
        alg_matrix_add_inplace(handle->position, handle->vec);
        alg_matrix_clamp_vecs(handle->position, handle->optim.l_range, handle->optim.r_range, SET_ROW);
        alg_matrix_free(sub1);
        alg_matrix_free(sub2);
        optim_fresh(&handle->optim, handle->position, handle->fitness);
    }
    alg_vector_free(best_position);
    alg_vector_free(current_position);
    return ALG_OK;
}

alg_state pso_free(pso_handle *handle) {
    pso_free_internal(handle);
    ALG_FREE(handle->p_best_fitness);
    ALG_FREE(handle);
    return ALG_OK;
}
