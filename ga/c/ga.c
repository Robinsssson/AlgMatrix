#include "ga.h"
#include <math.h>

ga_handle *ga_init(int pop_size, double mutation_rate, double crossover_rate, ga_aim_function function, int args_number, double var_max, double var_min) {
    ga_handle *ga = malloc(sizeof(ga_handle));
    if (ga == NULL)
        return NULL;
    ga->crossover_rate = crossover_rate;
    ga->mutation_rate = mutation_rate;
    ga->pop_size = pop_size;
    ga->function = function;
    ga->var_min = var_min;
    ga->var_max = var_max;
    alg_matrix *population = alg_matrix_create(pop_size, args_number);
    if (population == NULL) {
        free(ga);
        return NULL;
    }
    ga->population = population;
    for (int i = 0; i < pop_size; i++) {
        for (int j = 0; j < args_number; j++) {
            double value = alg_random_float64(var_min, var_max);
            alg_matrix_set_val(population, i, j, value);
        }
    }
    alg_vector *fitness = alg_vector_create(ga->pop_size, 0.0);
    if (fitness == NULL) {
        free(ga);
        free(population);
        return NULL;
    }
    ga->fitness = fitness;
    return ga;
}

static void calculate_fitness(ga_handle *ga) {
    for (int i = 0; i < ga->pop_size; i++) {
        alg_vector *vec = alg_vector_from_matrix_col(ga->population, i);
        alg_vector_set_val(ga->fitness, i, ga->function(vec));
        free(vec);
    }
}

static int select_best(ga_handle *ga) {
    calculate_fitness(ga);
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
    return child;
}

static void mutate(alg_vector *vec, double min, double max, double mutation_rate) {
    for (int i = 0; i < 2; i++) {
        if (alg_random_float64(0.0, 1.0) < mutation_rate) {
            alg_vector_set_val(vec, i, alg_random_float64(min, max));
        }
    }
}

void ga_fresh(ga_handle *ga);
void ga_free(ga_handle *ga);