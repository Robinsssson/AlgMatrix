#include "ga.h"
#include <math.h>
#include <stdlib.h>

ga_handle *ga_init(int pop_size, double mutation_rate, double crossover_rate,
                   ga_aim_function function, int args_number, double var_max, double var_min) {
    ga_handle *ga = ALG_MALLOC(sizeof(ga_handle));
    if (ga == NULL)
        return NULL;
    ga->crossover_rate = crossover_rate;    // 交叉率
    ga->mutation_rate = mutation_rate;      // 变异率
    ga->pop_size = pop_size;                // 种群大小
    ga->function = function;                // fitness函数
    ga->var_min = var_min;                  // lb每个变量的下界
    ga->var_max = var_max;                  // ub每个变量的上界
    alg_matrix *population = alg_matrix_create(pop_size, args_number);
    if (population == NULL) {
        ALG_FREE(ga);
        return NULL;
    }
    ga->population = population;            // 种群
    alg_matrix_fill_random(ga->population, var_min, var_max);

    alg_vector *fitness = alg_vector_create(ga->pop_size, 0.0);
    if (fitness == NULL) {
        ALG_FREE(ga);
        ALG_FREE(population);
        return NULL;
    }
    ga->fitness = fitness;
    return ga;
}

static void calculate_fitness(ga_handle *ga) {
    for (int i = 0; i < ga->pop_size; i++) {
        alg_vector *vec = alg_vector_from_matrix_col(ga->population, i);
        alg_vector_set_val(ga->fitness, i, ga->function(vec));
        ALG_FREE(vec);
    }
}

static int select(ga_handle *ga) {
    double min = INFINITY, tmp;
    int index = 0;
    for (int i = 0; i < ga->pop_size; i++) {
        tmp = *alg_vector_get_val(ga->fitness, i);
        if (min > tmp) {
            min = tmp;
            index = i;
        }
    }
    return index;
}

static alg_vector *crossover(ga_handle *ga, alg_vector *parent1, alg_vector *parent2) {
    alg_vector *child = alg_vector_create(ga->args_number, 0.0);
    int number_parent = round(ga->pop_size * ga->crossover_rate);
    
    return child;
}

static void mutate(alg_vector *vec, double min, double max, double mutation_rate) {
    for (int i = 0; i < 2; i++) {
        if (alg_random_float64(0.0, 1.0) < mutation_rate) {
            alg_vector_set_val(vec, i, alg_random_float64(min, max));
        }
    }
}

void ga_fresh(ga_handle *ga) {
    select(ga);
    calculate_fitness(ga);
}
void ga_free(ga_handle *ga);
