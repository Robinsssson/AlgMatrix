#ifndef CSO_H
#define CSO_H

#include "../../basic_opti.h"

// 定义CSO句柄
typedef struct {
    int pop_size;              // 群体大小
    int dim;                   // 粒子维度
    double llim;               // 下界
    double rlim;               // 上界
    alg_matrix *position;      // 粒子位置矩阵
    alg_matrix *velocity;      // 粒子速度矩阵
    alg_vector *fitness;       // 粒子适应度
    double global_best_fitness; // 全局最优适应度
    alg_vector *global_best_position; // 全局最优位置
    optimization function;         // 优化目标函数
} cso_handle;

// 函数声明
cso_handle *cso_init(int pop_size, int dim, double llim, double rlim, optimization func);
alg_state cso_fresh(cso_handle *handle);
alg_state cso_free(cso_handle *handle);

#endif // CSO_H
