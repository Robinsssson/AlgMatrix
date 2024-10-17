#ifndef __ALG_RANDOM_H__
#define __ALG_RANDOM_H__
#include <math.h>
#include <stdlib.h>

int alg_random_int(int min, int max);
double alg_random_float64(double min, double max);
double alg_random_normal(double mu, double sigma);
#endif
