#include "../algmath.h"
#include "alg_vector.h"
#include <stdio.h>
#include <stdlib.h>

#define STR_NAME(i) str_##i
#define PRINT_MACRO(vec, str, method)                                                              \
    do {                                                                                           \
        char *str = alg_##method##_print_str(vec);                                                 \
        if (str) {                                                                                 \
            printf("%s", str);                                                                     \
            ALG_FREE(str);                                                                         \
        }                                                                                          \
    } while (0)

int main() {
    // 创建向量
    alg_vector *vec = alg_vector_create(30, 12.0f);
    if (!vec) {
        fprintf(stderr, "Failed to create vector.\n");
        return EXIT_FAILURE;
    }

    // 打印第一次向量内容
    PRINT_MACRO(vec, STR_NAME(1), vector);

    // 向量插入元素
    alg_vector_insert(vec, 3, 1212);

    // 打印第二次向量内容
    PRINT_MACRO(vec, STR_NAME(2), vector);

    alg_matrix *matrix = alg_matrix_from_vector(vec);
    PRINT_MACRO(matrix, STR_NAME(3), matrix);
    // 释放向量内存
    alg_vector_free(vec);

    return EXIT_SUCCESS;
}
