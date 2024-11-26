#ifndef __ALG_UTILS_H
#define __ALG_UTILS_H
#include "../alg_inc.h"
#include <limits.h>
#ifdef __cplusplus
extern "C" {
#endif
ALG_MATH_API int alg_utils_less(const void *a, const void *b);
ALG_MATH_API int alg_utils_greater(const void *a, const void *b);
#define ALG_ALL_RANGE INT_MAX
#ifdef __cplusplus
}
#endif
#endif //!__ALG_UTILS_H
