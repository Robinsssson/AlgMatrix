#include "alg_math.h"

double alg_math_safe_divide(double numerator, double denominator) {
    if (fabs(denominator) < 1e-9) {
        // 除数为零，返回 NaN (Not a Number)
        ERROR("Division by zero!");
        return NAN; // 返回 NaN，表示无效的结果
    }
    return numerator / denominator; // 正常除法
}
