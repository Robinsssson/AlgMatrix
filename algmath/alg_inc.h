#ifndef __ALG_INC_H__
#define __ALG_INC_H__
#include <assert.h>

#ifdef _WIN32     // Windows平台
#ifdef ALG_EXPORT // 定义在编译动态库时设置的宏
#define ALG_MATH_API __declspec(dllexport)
#else
#define ALG_MATH_API __declspec(dllimport)
#endif
#else // 非Windows平台
#define ALG_MATH_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 定义基础类型
typedef double alg_val_type;
#define ALG_INIT_VAL 0.0
typedef enum {
    ALG_OK,
    ALG_ERROR,
} alg_state;

// 启用断言
#define USE_ASSERT 1
#define USE_LOG 1

struct __alg_matrix {
    int row;
    int col;
    alg_val_type mat[]; // 一维数组存储矩阵数据
};

struct __alg_vector {
    int size;
    int caps;
    alg_val_type vector[];
};                          //动态数组

typedef struct __alg_vector alg_vector;
typedef struct __alg_matrix alg_matrix;
// 定义日志消息字符串池
#define ALG_LOG_MSG_CREATE_SUCCESS "Vector created successfully."
#define ALG_LOG_MSG_CREATE_FAILURE "Failed to allocate memory for vector."
#define ALG_LOG_MSG_INSERT_SUCCESS "Value inserted successfully."
#define ALG_LOG_MSG_INSERT_FAILURE "Failed to insert value, memory allocation issue."
#define ALG_LOG_MSG_OUT_OF_BOUNDS "Position out of bounds."
#define ALG_LOG_MSG_FREE_SUCCESS "Vector freed successfully."
#define ALG_LOG_MSG_FREE_NULL "Attempted to free a NULL vector."

#if USE_LOG
#define LOGGING(str)                                                                               \
    printf("%s, in file %s, function %s, line %d\n", str, __FILE__, __FUNCTION__, __LINE__);
#else
#define LOGGING(str)
#endif
typedef struct {
    void *(*alg_memalloc_malloc)(unsigned long long);
    void (*alg_memalloc_free)(void *);
    void *(*alg_memalloc_realloc)(void *, unsigned long long);
    void *(*alg_memalloc_calloc)(unsigned long long, unsigned long long);
} alg_memalloc_hook;


#ifdef __cplusplus
}
#endif

#endif
