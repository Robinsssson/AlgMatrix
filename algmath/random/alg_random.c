#include "alg_random.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define PI (3.1415926535)

int alg_random_int(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + rand() % (max - min);
}

double alg_random_float64(double min, double max) {
    if (min > max) {
        double temp = min;
        min = max;
        max = temp;
    }
    double scale = rand() / (double)RAND_MAX; // 生成 [0, 1) 的随机数
    return min + scale * (max - min);         // 平移到 [min, max]
}

double alg_random_normal(double mu, double sigma) {
    // 使用 Box-Muller 变换
    double u1 = rand() / (double)RAND_MAX; // [0, 1) 均匀分布
    double u2 = rand() / (double)RAND_MAX; // [0, 1) 均匀分布

    // Box-Muller 变换
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);

    // 调整为期望均值和标准差
    return mu + z0 * sigma;
}

alg_state alg_random_sample_unique(int range_l, int range_r, int number, int *ret_array) {
    // 参数校验
    if (range_r - range_l < number) { // 左闭右开区间，元素个数为 range_r - range_l
        return ALG_ERROR;             // 范围内元素不足
    }

    // 构建范围数组
    int size = range_r - range_l; // 元素总数
    int *temp_array = (int *)malloc((size_t)size * sizeof(int));
    if (!temp_array) {
        return ALG_ERROR; // 内存分配失败
    }

    for (int i = 0; i < size; i++) {
        temp_array[i] = range_l + i;
    }

    // Fisher-Yates 洗牌算法
    for (int i = size - 1; i > 0; i--) {
        int j = alg_random_int(0, i + 1); // 在 [0, i] 区间生成随机索引
        int temp = temp_array[i];
        temp_array[i] = temp_array[j];
        temp_array[j] = temp;
    }

    // 取前 number 个元素
    for (int i = 0; i < number; i++) {
        ret_array[i] = temp_array[i];
    }

    free(temp_array); // 释放临时数组
    return ALG_OK;
}
