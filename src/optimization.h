#ifndef __OPTIMIZATION_H
#define __OPTIMIZATION_H
/* *******************************************************
 * 遗传算法 - Genetic Algorithm (GA)
 * 粒子群算法 - Particle Swarm Optimization (PSO)
 * 蚁群算法 - Ant Colony Optimization (ACO)
 * 模拟退火算法 - Simulated Annealing (SA)
 * 鸡群算法 - Chicken Swarm Optimization (CSO)
 * 布谷鸟算法 - Cuckoo Search (CS)
 * 蜂群算法 - Artificial Bee Colony (ABC) Algorithm
 * 免疫算法 - Artificial Immune Algorithm (AIA)
 * 分布估计算法 - Estimation of Distribution Algorithm (EDA)
 * 差分进化算法 - Differential Evolution (DE)
 *********************************************************/
#include "../algmath/alg_inc.h"

enum optimization_method {
    GA,  // 遗传算法
    PSO, // 粒子群算法
    ACO, // 蚁群算法
    SA,  // 模拟退火算法
    CSO, // 鸡群算法
    CS,  // 布谷鸟算法
    ABC, // 蜂群算法
    AIA, // 免疫算法
    EDA, // 分布估计算法
    DE   // 差分进化算法
};

void optimization(enum optimization_method method);

#endif
