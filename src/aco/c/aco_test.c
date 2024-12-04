#include "aco.h"
#include <algmath.h>
#include <math.h>
#include <stdio.h>

// 定义城市数量
#define CITY_COUNT 8

// 定义城市坐标
static double city_coords_data[CITY_COUNT][2] = {
    {0.0, 0.0},   // 城市 0
    {22.0, 0.0},  // 城市 1
    {1.0, 100.0}, // 城市 2
    {0.0, 1.0},   // 城市 3
    {12.0, 0.0},  // 城市 0
    {1.0, 8.0},   // 城市 1
    {7.0, 100.0}, // 城市 2
    {5.0, 12.0}   // 城市 3
};

// 将城市坐标数据转化为 alg_matrix
static alg_matrix *create_city_coords_matrix(void) {
    alg_matrix *city_coords = alg_matrix_create(CITY_COUNT, 2);
    for (int i = 0; i < CITY_COUNT; i++) {
        alg_matrix_set_val(city_coords, i, 0, city_coords_data[i][0]);
        alg_matrix_set_val(city_coords, i, 1, city_coords_data[i][1]);
    }
    return city_coords;
}

static void test_aco(void) {
    // 1. 创建城市坐标矩阵
    alg_matrix *city_coords = create_city_coords_matrix();

    // 2. 初始化 ACO 算法参数
    double alpha = 1.0; // 信息素的重要性
    double beta = 2.0;  // 启发式信息的重要性
    double rho = 0.1;   // 信息素蒸发率
    int number = 10;    // 蚂蚁的数量
    int iter = 100;
    // 3. 初始化 ACO 算法
    aco_handle *aco = aco_init(number, city_coords, alpha, beta, rho);
    if (aco == NULL) {
        printf("ACO initialization failed!\n");
        return;
    }

    // 4. 执行 ACO 算法，寻找最短路径
    for (int it = 0; it < iter; it++) {
        if (aco_fresh(aco) != ALG_OK) {
            printf("ACO algorithm failed!\n");
            aco_free(aco);
            return;
        }
        // 5. 输出最佳路径和路径长度
        printf("Best path: ");
        for (int i = 0; i < CITY_COUNT; i++) {
            printf("%d ", (int)*alg_vector_get_val(aco->best_path, i));
        }
        printf("\n");

        printf("Best path length: %f\n", aco->best_length);
    }

    // 6. 释放 ACO 算法的资源
    aco_free(aco);
}

int main(void) {
    // 执行测试
    test_aco();
    return 0;
}
