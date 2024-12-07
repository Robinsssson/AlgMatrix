#ifndef __ALGMATH_H__
#define __ALGMATH_H__
#include "alg_inc.h"
#include "matrix/alg_matrix.h"
#include "memalloc/alg_memalloc.h"
#include "random/alg_random.h"
#include "utils/alg_utils.h"
#include "vector/alg_vector.h"

#include <math.h>

#define FLOAT_COMPARE_IS(val1, val2) (fabs(val1 - val2) < 1e-9)
#define MATH_CLAIM(x, min, max) (x > max ? max : (x < min ? min : x))
#define API_VERSION "0.0.1"
#endif
