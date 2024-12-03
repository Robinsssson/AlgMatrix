#include "ga.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define GA_LOGGING(fmt, ...)                                                                                           \
    do {                                                                                                               \
        printf(fmt, ##__VA_ARGS__);                                                                                    \
    } while (0)

#define GA_LOGGING_MAT(mat)                                                                                            \
    do {                                                                                                               \
        char *strs = alg_matrix_print_str(mat);                                                                        \
        printf("%s\n", strs);                                                                                          \
        ALG_FREE(strs);                                                                                                \
    } while (0)
// 初始化遗传算法结构体
ga_handle *ga_init(int pop_size, double mutation_rate, double crossover_rate, ga_aim_function function, int args_number,
                   double var_max, double var_min) {
    ga_handle *ga = ALG_MALLOC(sizeof(ga_handle));
    if (ga == NULL)
        return NULL;

    ga->crossover_rate = crossover_rate; // 交叉率
    ga->mutation_rate = mutation_rate;   // 变异率
    ga->pop_size = pop_size;             // 种群大小
    ga->function = function;             // 适应度函数
    ga->var_min = var_min;               // 变量最小值
    ga->var_max = var_max;               // 变量最大值
    ga->args_number = args_number;       // 每个个体的基因长度（即参数个数）

    // 初始化种群矩阵
    alg_matrix *population = alg_matrix_create(pop_size, args_number);
    if (population == NULL) {
        ALG_FREE(ga);
        return NULL;
    }
    ga->population = population;                              // 种群矩阵
    alg_matrix_fill_random(ga->population, var_min, var_max); // 用随机值填充种群

    // 初始化适应度向量
    alg_vector *fitness = alg_vector_create(ga->pop_size, 0.0);
    if (fitness == NULL) {
        ALG_FREE(ga);
        alg_matrix_free(population);
        return NULL;
    }
    ga->fitness = fitness; // 适应度向量

    return ga;
}
int ga_select(ga_handle *ga) {
    int index = 0; // 初始化 index
    double tmp = ga->fitness->vector[0];
    for (int i = 1; i < ga->pop_size; i++) { // 修改为 i++，而非 ga++
        if (ga->fitness->vector[i] < tmp) {
            tmp = ga->fitness->vector[i];
            index = i;
        }
    }
    return index;
}
// 计算种群的适应度
static void calculate_fitness(ga_handle *ga) {
    for (int i = 0; i < ga->pop_size; i++) {
        alg_vector *vec = alg_vector_from_matrix_row(ga->population, i);
#ifdef USE_ASSERT
        assert(vec != NULL);
#else
        if (vec == NULL) {
            ERROR("there is a null vector");
            return;
        }
#endif
        alg_vector_set_val(ga->fitness, i, ga->function(vec)); // 计算每个个体的适应度
        alg_vector_free(vec);                                  // 释放临时的向量
    }
}

static alg_state sort_base_on_fitness(ga_handle *ga) {
    alg_vector *tmp_out = alg_vector_create_like(ga->fitness);
    if (tmp_out == NULL) {
        ERROR("THE VAL 'tmp_out' INIT ERROR");
        return ALG_ERROR;
    }
    int array[ga->pop_size];
    // base on fitness to copy in tmp_out and get index of array
    alg_vector_sort_copy(ga->fitness, tmp_out, array, alg_utils_greater);
    alg_matrix *copy_population = alg_matrix_copy(ga->population);
    if (copy_population == NULL) {
        ERROR("COPY POPULATION IS FAIL");
        alg_vector_free(tmp_out);
        return ALG_ERROR;
    }
    for (int i = 0; i < ga->pop_size; i++) {
        for (int j = 0; j < ga->args_number; j++) {
            alg_matrix_set_val(ga->population, i, j, *alg_matrix_get_pos_val(copy_population, array[i], j));
        }
    }
    for (int i = 0; i < tmp_out->size; i++)
        alg_vector_set_val(ga->fitness, i, tmp_out->vector[i]);
    alg_vector_free(tmp_out);
    alg_matrix_free(copy_population);
    calculate_fitness(ga);
    return ALG_OK;
}

// 交叉操作：生成两个子代
static alg_state crossover(ga_handle *ga, const alg_vector *parent1, const alg_vector *parent2, alg_vector **child1,
                           alg_vector **child2) {
    int cross_number = alg_random_int(1, ga->args_number); // 随机选择交叉点
    alg_state state;
    // 将父代划分为两部分
    alg_vector *tmp_parent1_end = alg_vector_slice(parent1, cross_number, ALG_ALL_RANGE);
    alg_vector *tmp_parent2_end = alg_vector_slice(parent2, cross_number, ALG_ALL_RANGE);
    alg_vector *tmp_parent1_begin = alg_vector_slice(parent1, ALG_ALL_RANGE, cross_number);
    alg_vector *tmp_parent2_begin = alg_vector_slice(parent2, ALG_ALL_RANGE, cross_number);
    if (tmp_parent1_begin == NULL || tmp_parent2_begin == NULL) {
        ERROR("ERROR");
    }
    if (tmp_parent1_end == NULL || tmp_parent2_end == NULL || tmp_parent1_begin == NULL || tmp_parent2_begin == NULL) {
        ERROR("CREARE VECTOR SLICE IS ERROR");
        return ALG_ERROR;
    }

    // 交换部分基因，生成子代
    state = alg_vector_concat_inplace(&tmp_parent1_begin, tmp_parent2_end, ALG_VECTOR_CONCAT_R);
    if (state == ALG_ERROR) {
        ERROR("CHILD CREATE ERROR");
        return ALG_ERROR;
    }
    state = alg_vector_concat_inplace(&tmp_parent2_begin, tmp_parent1_end, ALG_VECTOR_CONCAT_R);
    if (state == ALG_ERROR) {
        ERROR("CHILD CREATE ERROR");
        return ALG_ERROR;
    }
    *child1 = tmp_parent1_begin;
    *child2 = tmp_parent2_begin;

    // 释放临时向量
    alg_vector_free(tmp_parent1_end);
    alg_vector_free(tmp_parent2_end);

    return ALG_OK;
}

// 变异操作：对子代进行变异
static void mutate(ga_handle *ga, alg_vector *vector) {
    if (alg_random_float64(0, 1) < ga->mutation_rate) {
        int mutation_point = alg_random_int(0, ga->args_number + 1); // 随机选择变异点
        // 在变异点处用一个随机值进行变异
        alg_vector_set_val(vector, mutation_point, alg_random_float64(ga->var_min, ga->var_max));
    }
}

// 生成新一代种群
static alg_state generate_new_population(ga_handle *ga) {
    // 按照适应度对种群进行排序，选择优秀的父代
    sort_base_on_fitness(ga);

    // 选择父代的数量，这里我们选择偶数个父代
    int number_parents = (int)(round(ga->crossover_rate * ga->pop_size) / 2) * 2;
    // 新种群的矩阵，大小是父代数量的两倍（每对父代生成两个子代）
    alg_matrix *new_population = alg_matrix_create(number_parents * 2, ga->args_number);
    if (new_population == NULL) {
        ERROR("NEW POPULATION CREATE ERROR");
        return ALG_ERROR;
    }

    // 用于存放子代的数组
    alg_vector *child_list[2 * number_parents];

    // 开始交叉并生成子代
    for (int i = 0; i < number_parents; i += 2) {
        // 从种群中选择两个父代
        alg_vector *parent1 = alg_vector_from_matrix_row(ga->population, i);
        alg_vector *parent2 = alg_vector_from_matrix_row(ga->population, i + 1);
        if (parent1 == NULL || parent2 == NULL) {
            exit(-1);
        }
        // 进行交叉生成两个子代
        alg_vector *child1 = NULL, *child2 = NULL;
        if (crossover(ga, parent1, parent2, &child1, &child2) != ALG_OK) {
            alg_vector_free(parent1);
            alg_vector_free(parent2);
            return ALG_ERROR;
        }

        // 进行变异操作
        mutate(ga, child1);
        mutate(ga, child2);

        // 存储子代
        child_list[i] = child1;
        child_list[i + 1] = child2;

        // 释放父代向量
        alg_vector_free(parent1);
        alg_vector_free(parent2);
    }

    // 将交叉和变异后的子代填充到新种群
    for (int i = 0; i < number_parents; i += 2) {
        for (int j = 0; j < ga->args_number; j++) {
            alg_matrix_set_val(new_population, i, j, child_list[i]->vector[j]);
            alg_matrix_set_val(new_population, i + 1, j, child_list[i + 1]->vector[j]);
        }
    }

    // 选择精英策略：将适应度最好的父代保存到新种群中
    for (int i = number_parents; i < 2 * number_parents; i++) {
        for (int j = 0; j < ga->args_number; j++) {
            // 保留精英父代（最好适应度的个体）
            alg_matrix_set_val(new_population, i, j, *alg_matrix_get_pos_val(ga->population, i - number_parents, j));
        }
    }

    // 替换旧种群为新种群
    alg_matrix_free(ga->population);
    ga->population = new_population;
    ga->pop_size = ga->population->row;
    return ALG_OK;
}

// 主遗传算法循环，更新种群
void ga_fresh(ga_handle *ga) {
    calculate_fitness(ga);       // 计算适应度
    generate_new_population(ga); // 生成新一代种群
}

// 释放遗传算法的内存
void ga_free(ga_handle *ga) {
    if (ga == NULL)
        return;

    if (ga->population != NULL)
        alg_matrix_free(ga->population); // 释放种群矩阵

    if (ga->fitness != NULL)
        alg_vector_free(ga->fitness); // 释放适应度向量

    ALG_FREE(ga); // 释放GA结构体内存
}
