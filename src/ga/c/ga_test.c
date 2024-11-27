#include "ga.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// 示例适应度函数：目标是最大化 x^2 + y^2
double fitness_function(alg_vector *individual) {
    double x = *alg_vector_get_val(individual, 0); // 获取第一个基因
    double y = *alg_vector_get_val(individual, 1); // 获取第二个基因
    double z = *alg_vector_get_val(individual, 2); // 获取第二个基因
    double k = *alg_vector_get_val(individual, 3); // 获取第二个基因
    double l = *alg_vector_get_val(individual, 4); // 获取第二个基因
    double m = *alg_vector_get_val(individual, 5); // 获取第二个基因

    return (x * x + y * y + z * z + k * k + l * l + m * m);                       // 目标是最大化 x^2 + y^2
}

// 主函数：初始化GA并运行
int main() {
    // 遗传算法参数
    int pop_size = 100;          // 种群大小
    double mutation_rate = 0.01; // 变异率
    double crossover_rate = 0.5; // 交叉率
    int args_number = 6;         // 每个个体的基因数（假设我们有x和y两个基因）
    double var_max = 10.0;       // 基因的最大值
    double var_min = -10.0;      // 基因的最小值
    srand(time(0));
    // 初始化GA
    ga_handle *ga = ga_init(pop_size, mutation_rate, crossover_rate, fitness_function, args_number, var_max, var_min);
    if (ga == NULL) {
        printf("遗传算法初始化失败！\n");
        return -1;
    }
    LOGGING("GA ALG INIT OK");
    // 运行遗传算法，进行若干代进化
    int generations = 2000; // 迭代次数
    for (int gen = 0; gen < generations; gen++) {
        ga_fresh(ga); // 进行一代进化

        // 每10代输出一次最优个体
        if (gen % 10 == 0) {
            int best_index = ga_select(ga); // 获取最好的个体
            alg_vector *best_individual = alg_vector_from_matrix_row(ga->population, best_index);
            double best_fitness = *alg_vector_get_val(ga->fitness, best_index);
            printf("GEN: %d, Best fitness: %.6f, Best Solve: ", gen, best_fitness);
            for (int i = 0; i < args_number; i++) {
                printf("%.2f ", *alg_vector_get_val(best_individual, i));
            }
            printf("\n");
            alg_vector_free(best_individual); // 释放临时的最优个体向量
        }
    }

    // 获取最终最优解
    int best_index = ga_select(ga);
    alg_vector *best_individual = alg_vector_from_matrix_row(ga->population, best_index);
    double best_fitness = *alg_vector_get_val(ga->fitness, best_index);
    printf("\nBest Solve: ");
    for (int i = 0; i < args_number; i++) {
        printf("%.2f ", *alg_vector_get_val(best_individual, i));
    }
    printf("\nBest fitness: %.6f\n", best_fitness);

    // 释放遗传算法资源
    ga_free(ga);

    return 0;
}
