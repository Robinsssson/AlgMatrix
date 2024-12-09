#include "pso.h"
#include "algmath.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Helper function to ALG_FREE PSO resources
static void pso_free_internal(pso_handle *pso_structure) {
    alg_matrix_free(pso_structure->g_best);
    alg_matrix_free(pso_structure->p_best);
    alg_matrix_free(pso_structure->position);
    alg_matrix_free(pso_structure->vec);
    ALG_FREE(pso_structure->p_best_fitness);
}

pso_handle *pso_init(int number, int dim, double w, double c1, double c2, optimization function, pso_type type) {
    pso_handle *pso_structure = ALG_MALLOC(sizeof(pso_handle));
    if (!pso_structure)
        return NULL;

    pso_structure->vec = alg_matrix_create(number, dim);
    pso_structure->position = alg_matrix_create(number, dim);
    pso_structure->p_best = alg_matrix_copy(pso_structure->position);
    pso_structure->g_best = alg_matrix_create(number, dim); // Global best is usually a single particle

    if (!pso_structure->vec || !pso_structure->position || !pso_structure->p_best || !pso_structure->g_best) {
        pso_free_internal(pso_structure);
        return NULL;
    }

    pso_structure->p_best_fitness = ALG_MALLOC((long long unsigned int)number * sizeof(double));
    if (!pso_structure->p_best_fitness) {
        pso_free_internal(pso_structure);
        return NULL;
    }

    pso_structure->g_best_index = 0;
    pso_structure->g_best_fitness = (type == PSO_USE_MAX) ? -INFINITY : INFINITY;
    pso_structure->func = function;
    pso_structure->c1 = c1;
    pso_structure->c2 = c2;
    pso_structure->dim = dim;
    pso_structure->number = number;
    pso_structure->w = w;
    pso_structure->type = type;
    alg_matrix_fill_random(pso_structure->position, -5, 5);
    alg_matrix_fill_random(pso_structure->vec, -1, 1);
    return pso_structure;
}

// Helper function to calculate fitness for a particle
static double pso_calculate_fitness(pso_handle *pso_structure, int index) {
    alg_vector *vec = alg_vector_from_matrix_row(pso_structure->position, index);
    double fitness = pso_structure->func(vec);
    alg_vector_free(vec);
    return fitness;
}

void pso_fresh(pso_handle *pso_structure, double min_vec, double max_vec) {
    double best_val = (pso_structure->type == PSO_USE_MAX) ? -INFINITY : INFINITY;
    int best_idx = 0;

    // 遍历每个粒子
    for (int i = 0; i < pso_structure->number; i++) {
        double fitness = pso_calculate_fitness(pso_structure, i);

        // 更新个人最佳位置和适应度
        if ((pso_structure->type == PSO_USE_MAX && fitness > pso_structure->p_best_fitness[i])
            || (pso_structure->type == PSO_USE_MIN && fitness < pso_structure->p_best_fitness[i])) {
            // 更新个人最佳适应度
            pso_structure->p_best_fitness[i] = fitness;
            // 更新个人最佳位置
            alg_vector *current_position = alg_vector_from_matrix_row(pso_structure->position, i);
            alg_matrix_set_row(pso_structure->p_best, i, current_position);
            alg_vector_free(current_position);
        }

        // 更新全局最佳
        if ((pso_structure->type == PSO_USE_MAX && fitness > best_val)
            || (pso_structure->type == PSO_USE_MIN && fitness < best_val)) {
            best_val = fitness;
            best_idx = i;
        }
    }

    // 更新全局最佳位置和适应度
    if (best_idx != pso_structure->g_best_index) {
        alg_vector *best_position = alg_vector_from_matrix_row(pso_structure->position, best_idx);
        for (int i = 0; i < pso_structure->number; i++)
            alg_matrix_set_row(pso_structure->g_best, i, best_position); // g_best 是单个粒子的位置
        alg_vector_free(best_position);
        pso_structure->g_best_fitness = best_val;
        pso_structure->g_best_index = best_idx;
    }

    // 更新粒子速度和位置
    alg_matrix_dot_number_inplace(pso_structure->vec, pso_structure->w);

    alg_matrix *sub1 = alg_matrix_subtraction(pso_structure->p_best, pso_structure->position);
    alg_matrix *sub2 = alg_matrix_subtraction(pso_structure->g_best, pso_structure->position);

    pso_structure->r1 = alg_random_float64(0, 1);
    pso_structure->r2 = alg_random_float64(0, 1);

    alg_matrix_dot_number_inplace(sub1, pso_structure->r1 * pso_structure->c1);
    alg_matrix_dot_number_inplace(sub2, pso_structure->r2 * pso_structure->c2);

    alg_matrix_add_inplace(pso_structure->vec, sub1);
    alg_matrix_add_inplace(pso_structure->vec, sub2);
    alg_matrix_clamp(pso_structure->vec, min_vec, max_vec);
    alg_matrix_add_inplace(pso_structure->position, pso_structure->vec);

    alg_matrix_free(sub1);
    alg_matrix_free(sub2);
}

pso_handle *pso_free(pso_handle *pso_structure) {
    if (pso_structure) {
        pso_free_internal(pso_structure);
        ALG_FREE(pso_structure);
    }
    return NULL;
}
