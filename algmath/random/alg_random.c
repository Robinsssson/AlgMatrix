#include "alg_random.h"
#include <math.h>
#include <stdlib.h>


int alg_random_int(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + rand() % (max - min + 1);
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
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

    // 调整为期望均值和标准差
    return mu + z0 * sigma;
}