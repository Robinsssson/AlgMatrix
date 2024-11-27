#include "alg_vector.h"
#include "../matrix/alg_matrix.h"
#include "../memalloc/alg_memalloc.h"
#include "../utils/alg_utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

alg_vector *alg_vector_create(int size, alg_val_type init_val) {
    int t = ceil((double)size / ALG_VECTOR_BASE_SIZE);
    alg_vector *vec = ALG_MALLOC(sizeof(alg_vector) + sizeof(alg_val_type) * t * ALG_VECTOR_BASE_SIZE);
    if (vec == NULL)
        return NULL;

    vec->caps = t * ALG_VECTOR_BASE_SIZE;
    vec->size = size;

    for (int i = 0; i < size; i++)
        vec->vector[i] = init_val;

    return vec;
}

const alg_val_type *alg_vector_get_val(const alg_vector *vec, int pos) {
#if USE_ASSERT
    assert(vec != NULL);
#else
    if (vec == NULL)
        return NULL;
#endif
    if (pos >= vec->size)
        return NULL;
    return &vec->vector[pos];
}

alg_state alg_vector_set_val(alg_vector *vec, int pos, alg_val_type val) {
#if USE_ASSERT
    assert(vec != NULL);
#else
    if (vec == NULL)
        return ALG_ERROR;
#endif
    if (pos >= vec->size)
        return ALG_ERROR;

    vec->vector[pos] = val;
    return ALG_OK;
}

alg_state alg_vector_insert(alg_vector **vec, int pos, alg_val_type val) {
#if USE_ASSERT
    assert(*vec != NULL);
#else
    if (*vec == NULL)
        return ALG_ERROR;
#endif
    if (pos > (*vec)->size)
        return ALG_ERROR;

    // Check if the vector needs to be resized
    if ((*vec)->size >= (*vec)->caps) {
        int new_caps = (*vec)->caps + ALG_VECTOR_BASE_SIZE;

        // Create a new vector with expanded capacity
        alg_vector *new_vec = (alg_vector *)malloc(sizeof(alg_vector) + new_caps * sizeof(alg_val_type));
        if (new_vec == NULL)
            return ALG_ERROR; // Memory allocation failed

        // Copy the old data to the new vector
        memcpy(new_vec->vector, (*vec)->vector, (*vec)->size * sizeof(alg_val_type));
        new_vec->size = (*vec)->size;
        new_vec->caps = new_caps;

        // Free the old vector's memory and update the original vector pointer
        free((*vec)->vector); // Free the old vector's array memory
        free(*vec);           // Free the old vector structure

        *vec = new_vec; // Update the original pointer to the new vector
    }

    // Move elements to the right to make space for the new value
    memmove(&(*vec)->vector[pos + 1], &(*vec)->vector[pos], ((*vec)->size - pos) * sizeof(alg_val_type));
    // Insert the new value
    (*vec)->vector[pos] = val;
    (*vec)->size++;
    return ALG_OK;
}

alg_state alg_vector_free(alg_vector *vec) {
    if (vec == NULL)
        return ALG_ERROR;
    ALG_FREE(vec);
    return ALG_OK;
}

alg_vector *alg_vector_from_matrix_row(alg_matrix *matrix, int row) {
    alg_vector *vec = alg_vector_create(matrix->col, ALG_INIT_VAL);
    if (vec == NULL)
        return NULL;
    for (int i = 0; i < matrix->col; i++)
        alg_vector_set_val(vec, i, *alg_matrix_get_pos_val(matrix, row, i));
    return vec;
}

alg_vector *alg_vector_from_matrix_col(alg_matrix *matrix, int col) {
    alg_vector *vec = alg_vector_create(matrix->row, ALG_INIT_VAL);
    if (vec == NULL)
        return NULL;
    for (int i = 0; i < matrix->row; i++)
        alg_vector_set_val(vec, i, *alg_matrix_get_pos_val(matrix, i, col));
    return vec;
}

char *alg_vector_print_str(const alg_vector *vector) {
    if (vector == NULL)
        return NULL;
    int buf_size = vector->size * 25 + 10;
    char *str = (char *)ALG_MALLOC(buf_size);
    int pos = 0;
    pos += snprintf(str + pos, buf_size - pos, "[");
    for (int i = 0; i < vector->size; i++) {
        pos += snprintf(str + pos, buf_size - pos, "%.2f", vector->vector[i]);
        if (i != vector->size - 1)
            pos += snprintf(str + pos, buf_size - pos, " ");
    }
    pos += snprintf(str + pos, buf_size - pos, "]\n");
    return str;
}

alg_state alg_vector_sort_copy(const alg_vector *src_vector, alg_vector *dest_vector, int *sort_index,
                               int (*ptr_compare)(const void *, const void *)) {
    // Ensure the destination vector has enough space to hold the source data
    if (dest_vector->caps < src_vector->size) {
        // wait to fill
    }

    // Copy data from source to destination
    dest_vector->size = src_vector->size;
    memcpy(dest_vector->vector, src_vector->vector, sizeof(alg_val_type) * src_vector->size);

    // Sort the destination vector
    qsort(dest_vector->vector, (size_t)dest_vector->size, sizeof(alg_val_type), ptr_compare);

    // If sort_index is provided, fill it with the original indices
    if (sort_index != NULL) {
        for (int i = 0; i < dest_vector->size; i++) {
            for (int j = 0; j < src_vector->size; j++) {
                // Floating-point comparison with tolerance
                if (fabs(dest_vector->vector[i] - src_vector->vector[j]) < 1e-9) {
                    sort_index[i] = j;
                    break;
                }
            }
        }
    }

    return ALG_OK;
}
// {1, 2, 3, 4, 5, 6} size = 6
// [1:2] {2}
// [1:5] {2, 3, 4, 5}
// [1:6] {2, 3, 4, 5, 6}
// [-2:-1] {5} => [5:6]
alg_vector *alg_vector_slice(const alg_vector *vector, int range_l, int range_r) {
    // 默认处理“全部范围”的情况
    if (range_l == ALG_ALL_RANGE)
        range_l = 0;
    if (range_r == ALG_ALL_RANGE)
        range_r = vector->size;

    // 处理负索引
    if (range_l < 0)
        range_l = vector->size + range_l;
    if (range_r < 0)
        range_r = vector->size + range_r;

    // 检查索引有效性
    if (range_l < 0 || range_r < 0 || range_l >= vector->size || range_r > vector->size || range_l >= range_r)
        return NULL;

    // 计算切片的长度
    int len = range_r - range_l;

    // 创建返回的新向量
    alg_vector *ret_vec = alg_vector_create(len, 0);
    if (ret_vec == NULL)
        return NULL;

    // 复制源向量的切片到新向量
    memcpy(ret_vec->vector, vector->vector + range_l, len * sizeof(alg_val_type));

    return ret_vec;
}

alg_vector *alg_vector_create_like(const alg_vector *vector) {
    if (vector == NULL)
        return NULL;
    alg_vector *copy_vector = malloc(sizeof(alg_vector) + vector->caps * sizeof(alg_val_type));
    memcpy(copy_vector->vector, vector->vector, sizeof(alg_val_type) * vector->size);
    copy_vector->caps = vector->caps;
    copy_vector->size = vector->size;
    return copy_vector;
}

alg_state alg_vector_ensure_caps_inplace(alg_vector **ptr_vector) {
    if ((*ptr_vector)->caps < (*ptr_vector)->size) {
        // 计算新的容量，按 ALG_VECTOR_BASE_SIZE 的倍数进行扩展
        int new_caps = (((*ptr_vector)->size / ALG_VECTOR_BASE_SIZE) + 1) * ALG_VECTOR_BASE_SIZE;

        // 重新分配内存，扩展容量
        alg_vector *new_vec = realloc(*ptr_vector, sizeof(alg_vector) + new_caps * sizeof(alg_val_type));
        if (new_vec == NULL) {
            // 内存分配失败，返回错误
            return ALG_ERROR;
        }

        // 更新原始指针
        *ptr_vector = new_vec;
        (*ptr_vector)->caps = new_caps;
    }
    return ALG_OK;
}

alg_state alg_vector_concat_inplace(alg_vector **ptr_dest_vector, const alg_vector *src_vector, enum alg_vector_concat concat) {
    if (ptr_dest_vector == NULL || *ptr_dest_vector == NULL || src_vector == NULL)
        return ALG_ERROR; // 返回错误代码而不是 NULL

    // 创建目标向量的临时副本
    alg_vector *tmp_copy = alg_vector_create_like(*ptr_dest_vector);
    if (tmp_copy == NULL) {
        return ALG_ERROR; // 内存分配失败
    }

    // 增加目标向量的大小
    (*ptr_dest_vector)->size += src_vector->size;

    // 确保目标向量容量足够
    alg_state ret = alg_vector_ensure_caps_inplace(ptr_dest_vector);
    if (ret == ALG_ERROR) {
        alg_vector_free(tmp_copy); // 释放临时副本
        return ret;
    }

    if (concat == ALG_VECTOR_CONCAT_R) { // 右拼接
        for (int i = 0; i < src_vector->size; i++) {
            alg_vector_set_val(*ptr_dest_vector, (*ptr_dest_vector)->size - src_vector->size + i, *(alg_vector_get_val(src_vector, i)));
        }
    } else { // 左拼接
        for (int i = 0; i < (*ptr_dest_vector)->size; i++) {
            int label = i < src_vector->size;
            if (label) {
                alg_vector_set_val(*ptr_dest_vector, i, *(alg_vector_get_val(src_vector, i)));
            } else {
                alg_vector_set_val(*ptr_dest_vector, i, *(alg_vector_get_val(tmp_copy, i - src_vector->size)));
            }
        }
    }

    // 释放临时副本
    alg_vector_free(tmp_copy);

    return ALG_OK;
}
