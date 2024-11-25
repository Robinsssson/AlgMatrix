#include "pso.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// 定义PSO算法的目标函数
double test_function(alg_vector *vec) {
    // 确保向量的大小为2（二维空间）
    if (vec == NULL || vec->size != 2) {
        return -1;
    }

    double x = vec->vector[0];
    double y = vec->vector[1];

    // 计算目标函数 f(x, y) = (x - 1)^2 + (y - 2)^2
    return pow(x - 1, 2) + pow(y - 2, 2);
}

// 定义测试函数
void test_pso() {
    // PSO参数设定
    int particle_number = 500; // 粒子数量
    int dimensions = 2;       // 维度
    double w = 0.5;           // 惯性权重
    double c1 = 2.0;          // 个体学习因子
    double c2 = 2.0;          // 全局学习因子
    int max_iterations = 5000; // 最大迭代次数

    // 初始化PSO句柄，选择最小化问题
    pso_handle *pso = pso_init(particle_number, dimensions, w, c1, c2, test_function, PSO_USE_MIN);

    // PSO算法主循环
    for (int iter = 0; iter < max_iterations; iter++) {
        // 更新粒子位置和速度
        pso_fresh(pso, -1, 1);

        // 输出每次迭代的最优解和最优适应度值
        printf("Iteration %d: Best fitness = %.4f, Best position = (%.8f, %.8f)\n", iter,
               pso->g_best_fitness, pso->g_best->mat[0], pso->g_best->mat[1]);
    }

    // 检查最终结果
    printf("Final best fitness = %.4f, Final best position = (%.8f, %.8f)\n", pso->g_best_fitness,
           pso->g_best->mat[0], pso->g_best->mat[1]);

    // 释放PSO句柄
    pso_free(pso);
}

int main() {
    srand(time(NULL));
    test_pso();
    return 0;
}