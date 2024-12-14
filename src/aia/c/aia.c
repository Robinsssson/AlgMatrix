#include "aia.h"
#include "alg_inc.h"
#include "matrix/alg_matrix.h"
#include <math.h>
aia_handle *aia_init(optim_handle optim, int num_antibodies, int clone_factor, double mutation_rate) {
    aia_handle *handle = ALG_MALLOC(sizeof(aia_handle));
    if (handle == NULL) {
        ERROR("ERROR INIT IN AIA_HANDLE OBJECT");
        return NULL;
    }
    handle->population = alg_matrix_create(num_antibodies, optim.dim);
    if (handle->population == NULL) {
        ERROR("ERROR INIT IN POPULATION OBJECT");
        ALG_FREE(handle);
        return NULL;
    }
    handle->fitness = alg_vector_create(num_antibodies, 0.0);
    if (handle->fitness == NULL) {
        ERROR("ERROR INIT IN FITNESS OBJECT");
        alg_matrix_free(handle->population);
        ALG_FREE(handle);
        return NULL;
    }
    alg_matrix_fill_random_vecs(handle->population, optim.l_range, optim.r_range, SET_ROW);
    handle->num_antibodies = num_antibodies;
    handle->clone_factor = clone_factor;
    handle->mutation_rate = mutation_rate;
    handle->optim = optim;
    return handle;
}

// static void evaluate_fitness_(aia_handle *handle) {
//     alg_vector *_tmp_vec;
//     for (int row = 0; row < handle->population->row; row++) {
//         _tmp_vec = alg_vector_from_matrix_row(handle->population, row);
//         alg_vector_set_val(handle->fitness, row, handle->optim.function(_tmp_vec));
//         alg_vector_free(_tmp_vec);
//     }
// }

static alg_vector *evaluate_fitness(aia_handle *handle, alg_matrix *mat) {
    alg_vector *_tmp_vec;
    alg_vector *vecs = alg_vector_create(mat->row, 0.0);
    for (int row = 0; row < mat->row; row++) {
        _tmp_vec = alg_vector_from_matrix_row(mat, row);
        alg_vector_set_val(vecs, row, handle->optim.function(_tmp_vec));
        alg_vector_free(_tmp_vec);
    }
    return vecs;
}

static alg_matrix *clone_and_mutate(aia_handle *handle) {
    int len = handle->num_antibodies;
    int number_clones[len];

    // 防止除零溢出
    alg_vector *div_fitness = alg_vector_create_like(handle->fitness);
    for (int i = 0; i < div_fitness->size; i++) {
        div_fitness->vector[i] = 1 / (handle->fitness->vector[i] + 1e-6);
    }

    double sum_val = alg_vector_sum(div_fitness);
    int sum = 0;
    for (int i = 0; i < div_fitness->size; i++) {
        number_clones[i] = (int)ceil(handle->clone_factor * div_fitness->vector[i] / sum_val);
        sum += number_clones[i];
    }

    alg_matrix *clones = alg_matrix_create(sum, handle->optim.dim);
    int count = 0;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < number_clones[i]; j++) { // 修正为 number_clones[i]
            alg_vector *clone = alg_vector_from_matrix_row(handle->population, i);
            for (int iter = 0; iter < clone->size; iter++) {
                clone->vector[iter] += (handle->optim.r_range->vector[iter] - handle->optim.l_range->vector[iter])
                                       * handle->mutation_rate * alg_random_float64(-0.5, 0.5);
                clone->vector[iter] = MATH_CLAIM(clone->vector[iter], handle->optim.l_range->vector[iter],
                                                 handle->optim.r_range->vector[iter]);
                alg_matrix_set_val(clones, count, iter, clone->vector[iter]);
            }
            count++;
            alg_vector_free(clone);
        }
    }
    alg_vector_free(div_fitness); // 避免内存泄漏
    return clones;
}

static void select_new_population(aia_handle *handle, alg_matrix **clones) {
    alg_matrix_concat(clones, handle->population, CONCAT_AXIS_DY);
    alg_vector *fitness = evaluate_fitness(handle, *clones);
    alg_vector *copy_fitness = alg_vector_create_like(fitness);
    int sorted_indices[fitness->size];
    alg_vector_sort_copy(copy_fitness, fitness, sorted_indices, alg_utils_greater);
    for (int i = 0; i < handle->population->row; i++)
        for (int j = 0; j < handle->population->col; j++) {
            alg_matrix_set_val(handle->population, i, j, *alg_matrix_get_pos_val(*clones, sorted_indices[i], j));
        }
    alg_vector_free(fitness);
    alg_vector_free(copy_fitness);
}

alg_state aia_fresh(aia_handle *handle, int gen) {
    for (int __iter = 0; __iter < gen; __iter++) {
        // evaluate_fitness_(handle);
        // int index = alg_vector_compare_val(handle->fitness, alg_utils_greater);
        // double current_best_solve = handle->fitness->vector[index];
        // if (current_best_solve < handle->optim.bast_value) {
        //     handle->optim.bast_value = current_best_solve;
        //     for (int i = 0; i < handle->optim.dim; i++)
        //         handle->optim.best_solution->vector[i] = *alg_matrix_get_pos_val(handle->population, index, i);
        // }
        optim_fresh(&handle->optim, handle->population, handle->fitness);
        alg_matrix *clones = clone_and_mutate(handle);
        select_new_population(handle, &clones);
    }
    return ALG_OK;
}

alg_state aia_free(aia_handle *handle) {
    alg_vector_free(handle->fitness);
    alg_matrix_free(handle->population);
    ALG_FREE(handle);
    return ALG_OK;
}
