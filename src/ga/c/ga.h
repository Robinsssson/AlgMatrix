#ifndef __GA_H
#define __GA_H

#include "algmath.h"
typedef double (*ga_aim_function)(alg_vector *);
typedef struct {
    alg_matrix *population; // 种群矩阵，其表示[pop_size, args_number]的矩阵
    alg_vector *fitness;    // 适应度矩阵，其表示一个pop_size的适应度向量
    int pop_size, args_number;
    double mutation_rate, crossover_rate, var_max, var_min;
    ga_aim_function function;
} ga_handle;

ga_handle *ga_init(int pop_size, double mutation_rate, double crossover_rate, ga_aim_function function, int args_number,
                   double var_max, double var_min);
void ga_fresh(ga_handle *ga);
void ga_free(ga_handle *ga);
int ga_select(ga_handle *ga);
#endif
