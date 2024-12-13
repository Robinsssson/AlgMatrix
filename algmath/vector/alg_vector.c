#include "alg_vector.h"
#include "../matrix/alg_matrix.h"
#include "../memalloc/alg_memalloc.h"
#include "../utils/alg_utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

alg_vector *alg_vector_create(int size, alg_val_type init_val) {
    if (size <= 0) {
        ERROR("VECTOR CREATE SIZE IS ZERO OR NAGETIVE");
        return NULL;
    }
    int t = size / ALG_VECTOR_BASE_SIZE + 1;
    alg_vector *vec = ALG_MALLOC(sizeof(alg_vector) + sizeof(alg_val_type) * (unsigned int)t * ALG_VECTOR_BASE_SIZE);
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
    (*vec)->size++;
    alg_state state = alg_vector_ensure_caps_inplace(vec);
    if (state == ALG_ERROR)
        return state;
    // Move elements to the right to make space for the new value

    if ((*vec)->size <= pos) {
        ERROR("SIZE IS OUT OF POS");
        return ALG_ERROR;
    }
    size_t size_val = (size_t)((*vec)->size - pos);
    memmove(&(*vec)->vector[pos + 1], &(*vec)->vector[pos], size_val * sizeof(alg_val_type));
    // Insert the new value
    (*vec)->vector[pos] = val;
    return ALG_OK;
}

alg_state alg_vector_free(alg_vector *vec) {
    if (vec == NULL)
        return ALG_ERROR;
    ALG_FREE(vec);
    return ALG_OK;
}

alg_vector *alg_vector_from_matrix_row(alg_matrix *matrix, int row) {
    if (row >= matrix->row)
        return NULL;
    alg_vector *vec = alg_vector_create(matrix->col, ALG_INIT_VAL);
    if (vec == NULL)
        return NULL;
    for (int i = 0; i < matrix->col; i++)
        alg_vector_set_val(vec, i, *alg_matrix_get_pos_val(matrix, row, i));
    return vec;
}

alg_vector *alg_vector_from_matrix_col(alg_matrix *matrix, int col) {
    if (col >= matrix->col)
        return NULL;
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
    size_t buf_size = (size_t)vector->size * 25 + 10;
    char *str = (char *)ALG_MALLOC(buf_size);
    size_t pos = 0;
    pos += (size_t)snprintf(str + pos, buf_size - pos, "[");
    for (int i = 0; i < vector->size; i++) {
        pos += (size_t)snprintf(str + pos, buf_size - pos, "%.2f", vector->vector[i]);
        if (i != vector->size - 1)
            pos += (size_t)snprintf(str + pos, buf_size - pos, " ");
    }
    pos += (size_t)snprintf(str + pos, buf_size - pos, "]");
    return str;
}

alg_state alg_vector_sort_copy(alg_vector *dest_vector, const alg_vector *src_vector, int *sort_index,
                               int (*ptr_compare)(const void *, const void *)) {
    // Ensure the destination vector has enough space to hold the source data
    if (dest_vector->caps < src_vector->size) {
        ERROR("DEST VECTOR IS NOT CURRENT");
        return ALG_ERROR;
    }

    memcpy(dest_vector->vector, src_vector->vector, sizeof(alg_val_type) * (size_t)src_vector->size);

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

alg_state alg_vector_sort_inplace(alg_vector *src_vector, int *sort_index, alg_boolean change_flag,
                                  int (*ptr_compare)(const void *, const void *)) {
    alg_vector *dest_vector = alg_vector_create_like(src_vector);

    // Sort the destination vector
    if (change_flag == ALG_TRUE)
        qsort(src_vector->vector, (size_t)src_vector->size, sizeof(alg_val_type), ptr_compare);
    else
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
    alg_vector_free(dest_vector);
    return ALG_OK;
}

int alg_vector_compare_val(const alg_vector *src_vector, int (*ptr_compare)(const void *, const void *)) {
    int size = src_vector->size, index = 0;
    alg_val_type local_val = src_vector->vector[0];
    for (int i = 1; i < size; i++) {
        if (ptr_compare(&local_val, src_vector->vector + i) < 0)
            continue;
        index = i;
        local_val = *(src_vector->vector + i);
    }
    return index;
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
    if (range_l < 0 || range_r < 0 || range_l >= vector->size || range_r > vector->size || range_l >= range_r) {
        ERROR("SLICE VECTOR RANGE ERROR");
        return NULL;
    }

    // 计算切片的长度
    size_t len = (size_t)(range_r - range_l);

    // 创建返回的新向量
    alg_vector *ret_vec = alg_vector_create((int)len, 0);
    if (ret_vec == NULL) {
        ERROR("SLICE VECTOR CREATE INIT ERROR");
        return NULL;
    }

    // 复制源向量的切片到新向量
    memcpy(ret_vec->vector, vector->vector + range_l, len * sizeof(alg_val_type));

    return ret_vec;
}

alg_vector *alg_vector_create_like(const alg_vector *vector) {
    if (vector == NULL)
        return NULL;
    alg_vector *copy_vector = malloc(sizeof(alg_vector) + (size_t)vector->caps * sizeof(alg_val_type));
    memcpy(copy_vector->vector, vector->vector, sizeof(alg_val_type) * (size_t)vector->size);
    copy_vector->caps = vector->caps;
    copy_vector->size = vector->size;
    return copy_vector;
}

alg_state alg_vector_ensure_caps_inplace(alg_vector **ptr_vector) {
    if ((*ptr_vector)->caps == 0) {
        // 初始化容量为默认大小（例如 16）
        (*ptr_vector)->caps = ALG_VECTOR_BASE_SIZE;
    }

    if ((*ptr_vector)->caps <= (*ptr_vector)->size) {
        // 计算新的容量，按 ALG_VECTOR_BASE_SIZE 的倍数进行扩展
        int new_caps = (*ptr_vector)->caps;
        do {
            new_caps <<= 1;
            // 避免溢出
            if (new_caps <= (*ptr_vector)->caps) {
                return ALG_ERROR; // 内存溢出，返回错误
            }
        } while (new_caps < (*ptr_vector)->size);

        // 重新分配内存，扩展容量
        alg_vector *temp = ALG_REALLOC(*ptr_vector, sizeof(alg_vector) + (size_t)new_caps * sizeof(alg_val_type));
        if (temp == NULL) {
            // 内存分配失败，返回错误
            return ALG_ERROR;
        }
        *ptr_vector = temp; // 确保指针指向新分配的内存
        (*ptr_vector)->caps = new_caps;
    }
    return ALG_OK;
}

alg_state alg_vector_concat_inplace(alg_vector **ptr_dest_vector, const alg_vector *src_vector,
                                    enum alg_vector_concat concat) {
    if (ptr_dest_vector == NULL || *ptr_dest_vector == NULL || src_vector == NULL)
        return ALG_ERROR; // 返回错误代码而不是 NULL

    // 增加目标向量的大小
    (*ptr_dest_vector)->size += src_vector->size;

    // 确保目标向量容量足够
    alg_state ret = alg_vector_ensure_caps_inplace(ptr_dest_vector);
    if (ret == ALG_ERROR) {
        return ret;
    }

    if (concat == ALG_VECTOR_CONCAT_R) { // 右拼接
        for (int i = 0; i < src_vector->size; i++) {
            alg_vector_set_val(*ptr_dest_vector, (*ptr_dest_vector)->size - src_vector->size + i,
                               *(alg_vector_get_val(src_vector, i)));
        }
    } else { // 左拼接
        // 向右移动目标向量的元素
        for (int i = (*ptr_dest_vector)->size - 1; i >= src_vector->size; i--) {
            alg_vector_set_val(*ptr_dest_vector, i, *(alg_vector_get_val(*ptr_dest_vector, i - src_vector->size)));
        }
        // 插入源向量的元素
        for (int i = 0; i < src_vector->size; i++) {
            alg_vector_set_val(*ptr_dest_vector, i, *(alg_vector_get_val(src_vector, i)));
        }
    }

    return ALG_OK;
}

alg_val_type alg_vector_sum(const alg_vector *vec) {
    alg_val_type val = 0.0;
    for (int i = 0; i < vec->size; i++)
        val += vec->vector[i];
    return val;
}

alg_state alg_vector_claim(alg_vector *vec, alg_val_type l_range, alg_val_type r_range) {
    for (int i = 0; i < vec->size; i++) {
        vec->vector[i] = MATH_CLAIM(vec->vector[i], l_range, r_range);
    }
    return ALG_OK;
}

alg_state alg_vector_claim_vecs(alg_vector *vec, alg_vector *l_range, alg_vector *r_range) {
    for (int i = 0; i < vec->size; i++) {
        vec->vector[i] = MATH_CLAIM(vec->vector[i], l_range->vector[i], r_range->vector[i]);
    }
    return ALG_OK;
}
