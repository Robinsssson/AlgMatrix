#ifndef __ALG_INC_H__
#define __ALG_INC_H__
#include <assert.h>
#include <math.h>

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
}; // 动态数组

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
#include <stdio.h>

#if __STDC_VERSION__ >= 199901L
#define FUNC_NAME __func__
#else
#define FUNC_NAME "unknown_function"
#endif

#define MIRCO_PRINT_ARGS(logtag, fmt, ...)                                                                             \
    do {                                                                                                               \
        printf("[" logtag "] " fmt ", ", ##__VA_ARGS__);                                                               \
        printf("in file %s, function %s, line %d\n", __FILE__, FUNC_NAME, __LINE__);                                   \
    } while (0)
#define MIRCO_PRINT(logtag, fmt)                                                                                       \
    do {                                                                                                               \
        printf("[" logtag "] " fmt ", ");                                                                              \
        printf("in file %s, function %s, line %d\n", __FILE__, FUNC_NAME, __LINE__);                                   \
    } while (0)

#define LOGGING(fmt) MIRCO_PRINT("LOGGING", fmt)
#define LOGGING_ARGS(fmt, ...) MIRCO_PRINT_ARGS("LOGGING", fmt, ##__VA_ARGS__)

#define ERROR(fmt) MIRCO_PRINT("ERROR", fmt)
#define ERROR_ARGS(fmt, ...) MIRCO_PRINT_ARGS("ERROR", fmt, ##__VA_ARGS__)

#define TESTLOG(fmt)                                                                                                   \
    do {                                                                                                               \
        printf("\033[33m[TEST] " fmt "\033[0m\n");                                                                     \
    } while (0)
#define TESTLOG_ARGS(fmt, ...)                                                                                         \
    do {                                                                                                               \
        printf("\033[33m[TEST] " fmt "\033[0m\n", __VA_ARGS__);                                                        \
    } while (0)

#else
#define MIRCO_PRINT(logtag, fmt, ...)
#define LOGGING(fmt, ...) // 如果没有启用日志记录，空定义
#define TESTLOG_ARGS(fmt, ...)
#define ERROR(fmt, ...)
#define TESTLOG_SINGLE(fmt)
#endif

typedef struct {
    void *(*alg_memalloc_malloc)(size_t);
    void (*alg_memalloc_free)(void *);
    void *(*alg_memalloc_realloc)(void *, size_t);
    void *(*alg_memalloc_calloc)(size_t, size_t);
} alg_memalloc_hook;

#define FLOAT_COMPARE_IS(val1, val2) (fabs(val1 - val2) < 1e-9)
#define MATH_CLAIM(x, min, max) (x > max ? max : (x < min ? min : x))

typedef enum { ALG_FALSE = 0, ALG_TRUE = !ALG_FALSE } alg_boolean;

#ifdef __cplusplus
}
#endif

#endif
