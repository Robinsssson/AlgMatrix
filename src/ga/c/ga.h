#ifndef __GA_H
#define __GA_H

#include "../../basic_opti.h"

typedef struct {
    optim_handle optim;
    alg_matrix *population; // 种群矩阵，其表示[pop_size, args_number]的矩阵
    alg_vector *fitness;    // 适应度矩阵，其表示一个pop_size的适应度向量
    int pop_size;
    double mutation_rate, crossover_rate;

} ga_handle;

ALG_MATH_API ga_handle *ga_init(optim_handle optim, int pop_size, double mutation_rate, double crossover_rate);
ALG_MATH_API alg_state ga_fresh(ga_handle *handle, int gen);
ALG_MATH_API alg_state ga_free(ga_handle *handle);
#endif
