#ifndef __ALG_MEMALLOC_H
#define __ALG_MEMALLOC_H

#include "../alg_inc.h"
#ifdef __cplusplus
extern "C" {
#endif
ALG_MATH_API void *alg_calloc(unsigned long long number, unsigned long long size);
ALG_MATH_API void *alg_realloc(void *ptr, unsigned long long size);
ALG_MATH_API void alg_free(void *ptr);
ALG_MATH_API void *alg_malloc(unsigned long long size);
ALG_MATH_API void alg_memalloc_init(alg_memalloc_hook *handle);
#define ALG_MALLOC alg_malloc
#define ALG_FREE alg_free
#define ALG_REALLOC alg_realloc
#define ALG_CALLOC alg_calloc
#ifdef __cplusplus
}
#endif
#endif
