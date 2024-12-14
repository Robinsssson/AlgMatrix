#ifndef CSO_H
#define CSO_H

#include "../../basic_opti.h"

// 定义CSO句柄
typedef struct {
    int pop_size; // 群体大小
    int rn;       // 公鸡
    int hn;       // 母鸡
    int cn;       // 小鸡
    double fl;

    int *sorted_index;

    alg_matrix *position; // 粒子位置矩阵
    alg_vector *fitness;  // 粒子适应度

    optim_handle optim;
} cso_handle;

// 函数声明
cso_handle *cso_init(optim_handle optim, int pop_size, int rn, int hn, int cn, double fl);
alg_state cso_fresh(cso_handle *handle, int gen);
alg_state cso_free(cso_handle *handle);

#endif // CSO_H
