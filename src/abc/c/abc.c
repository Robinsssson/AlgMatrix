#include "abc.h"
#include "alg_inc.h"
#include "matrix/alg_matrix.h"
#include "memalloc/alg_memalloc.h"
#include "random/alg_random.h"
#include "vector/alg_vector.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

static void abc_fresh_fitness(abc_handle *handle);

abc_handle *abc_init(int food_number, int dimension, double lower_bound, double upper_bound, int limit, evaluate_function evaluate) {
    abc_handle *handle = ALG_MALLOC(sizeof(abc_handle));
    if (handle == NULL) {
        ERROR("HANDLE INIT ERROR");
        return NULL;
    }
    handle->food_number = food_number;
    handle->lower_bound = lower_bound;
    handle->upper_bound = upper_bound;
    handle->limit = limit;
    handle->evaluate = evaluate;

    handle->food_matrix = alg_matrix_create(food_number, dimension);
    if (handle->food_matrix == NULL) {
        ERROR("INIT FOOD_MATRIX ERROR");
        ALG_FREE(handle);
        return NULL;
    }
    alg_matrix_fill_random(handle->food_matrix, lower_bound, upper_bound);

    handle->fitness = alg_vector_create(food_number, 0.0);
    if (handle->fitness == NULL) {
    }
    abc_fresh_fitness(handle);

    handle->employed_bees = alg_matrix_copy(handle->food_matrix);
    handle->onlooker_bees = alg_matrix_copy(handle->food_matrix);

    return handle;
}

static void abc_fresh_fitness(abc_handle *handle) {
    for (int i = 0; i < handle->food_number; i++) {
        alg_vector *tmp_vector = alg_vector_from_matrix_row(handle->food_matrix, i);
        alg_vector_set_val(handle->fitness, i, handle->evaluate(tmp_vector));
        alg_vector_free(tmp_vector);
    }
}
static void search_new_food(abc_handle *handle, alg_vector *current_food) {
    alg_vector *step = alg_vector_create(handle->dim, 0.0);
    if (step == NULL) {
        ERROR("PHI INIT ERROR");
        return;
    }
    for (int i = 0; i < handle->dim; i++) {
        double current_val = *alg_vector_get_val(current_food, i);
        alg_vector_set_val(step, i, alg_random_float64(handle->lower_bound - current_val, current_val - handle->lower_bound));
        alg_vector_set_val(current_food, i, *alg_vector_get_val(step, i));
    }
    alg_vector_free(step);
}

static double vector_sum(alg_vector *vec) {
    double ret = 0.0;
    for (int i = 0; i < vec->size; i++)
        ret += vec->vector[i];
    return ret;
}

double alg_safe_divide(double numerator, double denominator) {
    if (fabs(denominator) < 1e-9) {
        // 除数为零，返回 NaN (Not a Number)
        ERROR("Division by zero!");
        return NAN; // 返回 NaN，表示无效的结果
    }
    return numerator / denominator; // 正常除法
}

int abc_get_best_solution(abc_handle *handle) {
    int index = 0;
    double fit = *alg_vector_get_val(handle->fitness, 0), fit_iterval;
    for (int i = 1; i < handle->food_number; i++) {
        fit_iterval = *alg_vector_get_val(handle->fitness, i);
        if (fit > fit_iterval) {
            fit = fit_iterval;
            index = i;
        }
    }
    alg_vector *best_vec = alg_vector_from_matrix_row(handle->food_matrix, index);
    char *strs = alg_vector_print_str(best_vec);
    printf("Best solve VAL is %.2f, Best SOLVE is %s", fit, strs);
    ALG_FREE(strs);
    alg_vector_free(best_vec);
    return index;
}

alg_state abc_fresh(abc_handle *handle) {
    alg_vector *new_food;
    double sum;
    double prob;
    double rand;
    double new_fitness;

    // 1. 雇佣蜜蜂搜索阶段
    for (int i = 0; i < handle->food_number; i++) {
        new_food = alg_vector_from_matrix_row(handle->employed_bees, i);
        search_new_food(handle, new_food);
        new_fitness = handle->evaluate(new_food);
        if (new_fitness < handle->fitness->vector[i]) {
            // 更新食物源和适应度
            alg_vector_set_val(handle->fitness, i, new_fitness);
            for (int iter = 0; iter < handle->dim; iter++) {
                alg_matrix_set_val(handle->food_matrix, i, iter, *alg_vector_get_val(new_food, iter));
            }
        }
        alg_vector_free(new_food);
    }

    // 2. 观察者蜜蜂搜索阶段
    sum = vector_sum(handle->fitness); // 计算适应度的总和
    for (int i = 0; i < handle->food_number; i++) {
        prob = alg_safe_divide(handle->fitness->vector[i], sum); // 计算选择该食物源的概率
        if (isnan(prob)) {
            ERROR("GET A NAN VAL");
            continue;
        }

        rand = alg_random_float64(0, 1); // 生成一个 0 到 1 之间的随机数
        if (rand > prob) {
            // 随机数大于概率值，不选择该食物源，跳过
            continue;
        }

        // 选择该食物源进行局部搜索
        new_food = alg_vector_from_matrix_row(handle->onlooker_bees, i);
        search_new_food(handle, new_food);
        new_fitness = handle->evaluate(new_food);
        if (new_fitness < handle->fitness->vector[i]) {
            // 更新食物源和适应度
            alg_vector_set_val(handle->fitness, i, new_fitness);
            for (int iter = 0; iter < handle->dim; iter++) {
                alg_matrix_set_val(handle->food_matrix, i, iter, *alg_vector_get_val(new_food, iter));
            }
        }
        alg_vector_free(new_food);
    }

    return ALG_OK;
}

alg_state abc_free(abc_handle *handle) {
    alg_matrix_free(handle->food_matrix);
    alg_matrix_free(handle->employed_bees);
    alg_matrix_free(handle->onlooker_bees);
    alg_vector_free(handle->fitness);
    ALG_FREE(handle);
    return ALG_OK;
}