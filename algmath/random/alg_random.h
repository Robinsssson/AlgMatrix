#ifndef __ALG_RANDOM_H__
#define __ALG_RANDOM_H__
#include "../alg_inc.h"

#ifdef __cplusplus
extern "C" {
#endif

ALG_MATH_API int alg_random_int(int min, int max);
ALG_MATH_API double alg_random_float64(double min, double max);
ALG_MATH_API double alg_random_normal(double mu, double sigma);
ALG_MATH_API alg_state alg_random_sample_unique(int range_l, int range_r, int number, int *ret_array);
#ifdef __cplusplus
}
#endif

#endif
