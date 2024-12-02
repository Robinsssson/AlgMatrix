#include "abc.h"
#include "alg_inc.h"
#include "vector/alg_vector.h"
#include <stdio.h>
#include <time.h>

// 定义多元 Rosenbrock 函数
static double rosenbrock(alg_vector *vec) {
    double sum = 0.0;
    // for (int i = 0; i < vec->size - 1; i++) {
    //     double x1 = *alg_vector_get_val(vec, i);
    //     double x2 = *alg_vector_get_val(vec, i + 1);
    //     sum += 100.0 * pow(x2 - x1 * x1, 2) + pow(1 - x1, 2);
    // }
    double x1 = *alg_vector_get_val(vec, 0);
    double x2 = *alg_vector_get_val(vec, 1);
    sum = x1 * x1 + x2 * x2;
    return sum;
}

// 测试用例
int main(void) {
    // 设置 ABC 参数
    int food_number = 500;     // 蜂群数量
    int dimension = 2;         // 搜索空间维度（此处为二维）
    double lower_bound = -5.0; // 搜索空间下界
    double upper_bound = 5.0;  // 搜索空间上界
    int limit = 200;           // 每个食物源的最大迭代次数

    // 初始化 ABC 算法
    abc_handle *handle = abc_init(food_number, dimension, lower_bound, upper_bound, limit, rosenbrock);
    if (handle == NULL) {
        ERROR("Failed to initialize ABC!");
        return -1;
    }

    // 记录开始时间
    clock_t start_time = clock();

    // 进行 ABC 算法的迭代更新
    for (int iter = 0; iter < 2000; iter++) { // 迭代1000次
        abc_fresh(handle);
        if (iter % 10 == 0) {
            printf("[gen %3d] ", iter / 10);
            abc_get_best_solution(handle);
        }
    }

    // 输出最优解
    abc_get_best_solution(handle);

    // 记录结束时间
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Optimization time: %.2f seconds\n", elapsed_time);

    // 清理资源
    abc_free(handle);

    return 0;
}
