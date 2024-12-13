#ifndef CSO_H
#define CSO_H

#include "../../basic_opti.h"

// 定义CSO句柄
typedef struct {
    int pop_size;              // 群体大小
    alg_matrix *position;      // 粒子位置矩阵
    alg_matrix *velocity;      // 粒子速度矩阵
    alg_vector *fitness;       // 粒子适应度
    optim_handle optim;
} cso_handle;

// 函数声明
cso_handle *cso_init(int pop_size, int dim, double llim, double rlim, optimization func);
alg_state cso_fresh(cso_handle *handle, int gen);
alg_state cso_free(cso_handle *handle);

#endif // CSO_H
