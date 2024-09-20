#ifndef __ALG_INC_H__
#define __ALG_INC_H__
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义基础类型
typedef double alg_val_type;
#define GLOBAL_INIT_VAL 0.0
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
    alg_val_type *mat; // 一维数组存储矩阵数据
};

struct __alg_vector {
    int size;
    int caps;
    alg_val_type *vector;
};

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
#define LOGGING(str) printf("%s, in file %s, line %d\n", str, __FILE__, __LINE__);
#else
#define LOGGING(str)
#endif

#endif
