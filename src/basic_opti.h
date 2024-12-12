#ifndef __BASIC_OPTI_H
#define __BASIC_OPTI_H
#include "algmath.h"

/*
 * function pointer for optimization
 */
typedef double (*optimization)(alg_vector *);
typedef struct {
    int dim;
    alg_vector *l_range, *r_range;
    optimization function;
} optim_handle;
#endif //!__BASIC_OPTI_H
