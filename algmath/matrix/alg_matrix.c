#include "alg_matrix.h"
#include "../alg_inc.h"
#include "../memalloc/alg_memalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

alg_matrix *alg_matrix_create(int row, int col) {
    if (row < 0 || col < 0) {
        ERROR("ERROR IN CREATE MATRIX WITH NAGETIVE ROW OR COL");
        return NULL;
    }
    alg_matrix *mat = ALG_MALLOC(sizeof(alg_matrix) + sizeof(alg_val_type) * (unsigned)row * (unsigned)col);
    if (mat == NULL)
        return NULL;

    mat->col = col;
    mat->row = row;
    return mat;
}

alg_matrix *alg_matrix_create_val(int row, int col, alg_val_type val) {
    if (row < 0 || col < 0) {
        ERROR("ERROR IN CREATE MATRIX WITH NAGETIVE ROW OR COL");
        return NULL;
    }
    alg_matrix *mat = ALG_MALLOC(sizeof(alg_matrix) + sizeof(alg_val_type) * (unsigned)row * (unsigned)col);
    if (mat == NULL)
        return NULL;
    for (int i = 0; i < row * col; i++)
        mat->mat[i] = val;
    mat->col = col;
    mat->row = row;
    return mat;
}

alg_state alg_matrix_set_val(alg_matrix *matrix, int row, int col, alg_val_type val) {
#if USE_ASSERT
    assert(matrix != NULL);
#else
    if (matrix == NULL)
        return ALG_ERROR;
#endif
    if (row >= matrix->row || col >= matrix->col)
        return ALG_ERROR; // 检查索引是否越界
    matrix->mat[row * matrix->col + col] = val;
    return ALG_OK;
}

const alg_val_type *alg_matrix_get_pos_val(const alg_matrix *matrix, int row, int col) {
#if USE_ASSERT
    assert(matrix != NULL);
#else
    if (matrix == NULL)
        return NULL;
#endif
    if (row >= matrix->row || col >= matrix->col)
        return NULL; // 检查索引是否越界
    return &matrix->mat[row * matrix->col + col];
}

ALG_MATH_API alg_val_type *alg_matrix_get_pos_mutval(alg_matrix *matrix, int row, int col) {
#if USE_ASSERT
    assert(matrix != NULL);
#else
    if (matrix == NULL)
        return NULL;
#endif
    if (row >= matrix->row || col >= matrix->col)
        return NULL; // 检查索引是否越界
    return &matrix->mat[row * matrix->col + col];
}

const alg_val_type *alg_matrix_get_index_val(const alg_matrix *matrix, int index) {
#if USE_ASSERT
    assert(matrix != NULL);
#else
    if (matrix == NULL)
        return NULL;
#endif
    if (index >= matrix->col * matrix->row)
        return NULL; // 检查索引是否越界
    return &matrix->mat[index];
}

alg_matrix *alg_matrix_copy(const alg_matrix *matrix) {
    alg_matrix *ret = alg_matrix_create(matrix->row, matrix->col);
    if (ret == NULL)
        return NULL;
    memcpy(ret->mat, matrix->mat, sizeof(alg_val_type) * (size_t)(matrix->col * matrix->row));
    return ret;
}

alg_matrix *alg_matrix_add(const alg_matrix *mat1, const alg_matrix *mat2) {
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat1->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return NULL;
    if (mat1->col != mat2->col || mat1->row != mat2->row)
        return NULL; // 检查行和列是否相等
#endif
    alg_matrix *ret = alg_matrix_create(mat1->row, mat1->col);
    if (ret == NULL)
        return NULL;

    for (int i = 0; i < mat1->col * mat1->row; i++)
        ret->mat[i] = mat1->mat[i] + mat2->mat[i];
    return ret;
}

alg_matrix *alg_matrix_subtraction(const alg_matrix *mat1, const alg_matrix *mat2) {
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat1->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return NULL;
    if (mat1->col != mat2->col || mat1->row != mat2->row)
        return NULL; // 检查行和列是否相等
#endif
    alg_matrix *ret = alg_matrix_create(mat1->row, mat1->col);
    if (ret == NULL)
        return NULL;

    for (int i = 0; i < mat1->col * mat1->row; i++)
        ret->mat[i] = mat1->mat[i] - mat2->mat[i];
    return ret;
}

alg_matrix *alg_matrix_dot(const alg_matrix *mat1, const alg_matrix *mat2) {
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat1->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return NULL;
    if (mat1->col != mat2->col || mat1->row != mat2->row)
        return NULL; // 检查行和列是否相等
#endif
    alg_matrix *ret = alg_matrix_create(mat1->row, mat1->col);
    if (ret == NULL)
        return NULL;

    for (int i = 0; i < mat1->col * mat1->row; i++)
        ret->mat[i] = mat1->mat[i] * mat2->mat[i];
    return ret;
}

alg_matrix *alg_matrix_dot_number(const alg_matrix *mat1, const double number) {
#if USE_ASSERT
    assert(mat1 != NULL);
#else
    if (mat1 == NULL)
        return NULL;
#endif
    alg_matrix *ret = alg_matrix_create(mat1->row, mat1->col);
    if (ret == NULL)
        return NULL;

    for (int i = 0; i < mat1->col * mat1->row; i++)
        ret->mat[i] = mat1->mat[i] * number;
    return ret;
}

alg_matrix *alg_matrix_times(alg_matrix *mat1, alg_matrix *mat2) {
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return NULL;
    if (mat1->col != mat2->row)
        return NULL; // 检查维度匹配
#endif
    alg_matrix *ret = alg_matrix_create(mat1->row, mat2->col);
    if (ret == NULL)
        return NULL;

    for (int i = 0; i < mat1->row; i++) {
        for (int j = 0; j < mat2->col; j++) {
            alg_val_type *val = alg_matrix_get_pos_mutval(ret, i, j);
            for (int k = 0; k < mat1->col; k++) {
                *val += (*alg_matrix_get_pos_val(mat1, i, k)) * (*alg_matrix_get_pos_val(mat2, k, j));
            }
        }
    }
    return ret;
}

alg_state alg_matrix_add_inplace(alg_matrix *mat1, const alg_matrix *mat2) {
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat1->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return ALG_ERROR;
    if (mat1->col != mat2->col || mat1->row != mat2->row)
        return ALG_ERROR;
#endif
    for (int i = 0; i < mat1->col * mat1->row; i++)
        mat1->mat[i] += mat2->mat[i];
    return ALG_OK;
}

alg_state alg_matrix_dot_inplace(alg_matrix *mat1, const alg_matrix *mat2) {
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat1->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return ALG_ERROR;
    if (mat1->col != mat2->col || mat1->row != mat2->row)
        return ALG_ERROR;
#endif
    for (int i = 0; i < mat1->col * mat1->row; i++)
        mat1->mat[i] *= mat2->mat[i];
    return ALG_OK;
}

alg_state alg_matrix_dot_number_inplace(alg_matrix *mat1, const double number) {
#if USE_ASSERT
    assert(mat1 != NULL);
#else
    if (mat1 == NULL)
        return ALG_ERROR;
#endif
    for (int i = 0; i < mat1->col * mat1->row; i++)
        mat1->mat[i] *= number;
    return ALG_OK;
}

alg_matrix *alg_matrix_from_vector(const alg_vector *vec) {
    alg_matrix *matrix = alg_matrix_create(vec->size, 1);
    if (matrix == NULL) {
        return NULL;
    }
    memcpy(matrix->mat, vec->vector, (size_t)vec->size * sizeof(alg_val_type));
    return matrix;
}

char *alg_matrix_print_str(alg_matrix *matrix) {
    if (matrix == NULL) {
        return NULL;
    }

    // 估算初始缓冲区大小。每个数字最多保留 2 位小数，含符号、逗号、空格等。
    size_t buf_size = (size_t)(matrix->row * matrix->col * 25 + matrix->row * 4 + 4); // 初始估算
    char *str = (char *)ALG_MALLOC(buf_size);
    if (str == NULL) {
        return NULL;
    }

    size_t pos = 0;
    pos += (size_t)snprintf(str + pos, buf_size - pos, "[\n"); // 开始矩阵输出

    for (int i = 0; i < matrix->row; i++) {
        pos += (size_t)snprintf(str + pos, buf_size - pos, " ["); // 每行的起始方括号
        for (int j = 0; j < matrix->col; j++) {
            // 每个元素以 "%.2f" 格式输出，后跟逗号
            pos += (size_t)snprintf(str + pos, buf_size - pos, "%.2f", matrix->mat[i * matrix->col + j]);
            if (j < matrix->col - 1) {
                pos += (size_t)snprintf(str + pos, buf_size - pos, ", ");
            }
        }
        pos += (size_t)snprintf(str + pos, buf_size - pos, "]"); // 行的结束方括号
        if (i < matrix->row - 1) {
            pos += (size_t)snprintf(str + pos, buf_size - pos, ",\n"); // 行之间的逗号换行
        }
    }
    pos += (size_t)snprintf(str + pos, buf_size - pos, "\n]\n"); // 矩阵结束方括号

    return str;
}

alg_state alg_matrix_free(alg_matrix *matrix) {
    if (matrix == NULL)
        return ALG_ERROR;
    ALG_FREE(matrix);
    return ALG_OK;
}

alg_matrix *alg_matrix_transpose(alg_matrix *mat) {
    alg_matrix *ret = alg_matrix_copy(mat);
    int tmp = ret->col;
    ret->col = ret->row;
    ret->row = tmp;
    // 将原矩阵的数据赋值到转置后的矩阵
    for (int i = 0; i < mat->row; i++) {
        for (int j = 0; j < mat->col; j++) {
            // 将原矩阵 mat[i][j] 的值赋给转置矩阵 ret[j][i]
            alg_matrix_set_val(ret, j, i, *alg_matrix_get_pos_val(mat, i, j));
        }
    }
    return ret;
}

alg_state alg_matrix_transpose_inplace(alg_matrix *mat) {
    int tmp = mat->col;
    mat->col = mat->row;
    mat->row = tmp;
    return ALG_OK;
}

alg_state alg_matrix_set_row(alg_matrix *matrix, int row, const alg_vector *vec) {
    // 检查传入的矩阵和向量是否为NULL
    if (matrix == NULL || vec == NULL)
        return ALG_ERROR;

    // 确保行号合法
    if (row < 0 || row >= matrix->row)
        return ALG_ERROR;

    // 确保向量大小与矩阵列数一致
    if (vec->size != matrix->col)
        return ALG_ERROR;

    // 将向量的值复制到矩阵的指定行
    for (int i = 0; i < matrix->col; i++) {
        matrix->mat[row * matrix->col + i] = vec->vector[i]; // 假设矩阵是行主序存储的
    }

    return ALG_OK;
}

void alg_matrix_fill_random(alg_matrix *matrix, double min_val, double max_val) {
    // 如果矩阵为空，直接返回
    if (matrix == NULL)
        return;

    int total_elements = matrix->row * matrix->col;

    // 生成 [min_val, max_val] 范围内的随机值并赋值给矩阵
    for (int i = 0; i < total_elements; i++) {
        double rand_val = min_val + (double)rand() / RAND_MAX * (max_val - min_val);
        matrix->mat[i] = rand_val;
    }
}

void alg_matrix_clamp(alg_matrix *matrix, double min_val, double max_val) {
    // 如果矩阵为空，直接返回
    if (matrix == NULL)
        return;

    int total_elements = matrix->row * matrix->col;

    // 遍历矩阵的每个元素并进行幅值限定
    for (int i = 0; i < total_elements; i++) {
        if (matrix->mat[i] < min_val)
            matrix->mat[i] = min_val;
        else if (matrix->mat[i] > max_val)
            matrix->mat[i] = max_val;
    }
}

#define INTERATOR_MATRIX(matrix, irow, icol)                                                                           \
    for (int irow = 0; irow < (matrix)->row; irow++)                                                                   \
        for (int icol = 0; icol < (matrix)->col; icol++)

alg_state alg_matrix_concat(alg_matrix **dest_matrix, alg_matrix *src_matrix, enum alg_matrix_concat concat) {
    if (dest_matrix == NULL || src_matrix == NULL) {
        return ALG_ERROR;
    }

    // 确定拼接方向并检查列或行数的匹配
    int is_row_match = ((*dest_matrix)->row == src_matrix->row);
    int is_col_match = ((*dest_matrix)->col == src_matrix->col);

    // 扩容计算（目标矩阵需要加上源矩阵的行/列）
    size_t new_size = 0;
    switch (concat) {
        case CONCAT_AXIS_LX: // 左侧拼接，行数相同，列数增加
        case CONCAT_AXIS_RX: // 右侧拼接，行数相同，列数增加
            if (!is_row_match)
                return ALG_ERROR;
            new_size = (size_t)((*dest_matrix)->row * ((*dest_matrix)->col + src_matrix->col)) * sizeof(alg_val_type)
                       + sizeof(alg_matrix);
            break;
        case CONCAT_AXIS_UY: // 上侧拼接，列数相同，行数增加
        case CONCAT_AXIS_DY: // 下侧拼接，列数相同，行数增加
            if (!is_col_match)
                return ALG_ERROR;
            new_size = (size_t)(((*dest_matrix)->row + src_matrix->row) * (*dest_matrix)->col) * sizeof(alg_val_type)
                       + sizeof(alg_matrix);
            break;
        default:
            return ALG_ERROR; // 无效的拼接方向
    }
    alg_matrix *copy_matrix = alg_matrix_copy(*dest_matrix);
    // 扩容目标矩阵
    *dest_matrix = ALG_REALLOC(*dest_matrix, new_size);
    // 更新目标矩阵的行数和列数
    if (concat == CONCAT_AXIS_LX || concat == CONCAT_AXIS_RX) {
        (*dest_matrix)->col += src_matrix->col;
    } else {
        (*dest_matrix)->row += src_matrix->row;
    }
    if (!dest_matrix)
        return ALG_ERROR; // 内存分配失败

    // 根据拼接方向执行数据搬移
    switch (concat) {
        case CONCAT_AXIS_LX: // 左侧拼接，行数相同，列数增加
            INTERATOR_MATRIX(*dest_matrix, i, j) {
                // 判断是否在源矩阵的列范围内
                if (j < src_matrix->col) {
                    // 如果在源矩阵列范围内，从源矩阵获取值
                    const alg_val_type val = *alg_matrix_get_pos_val(src_matrix, i, j);
                    alg_matrix_set_val(*dest_matrix, i, j, val);
                } else {
                    // 否则，从目标矩阵获取值，位置偏移源矩阵的列数
                    const alg_val_type val = *alg_matrix_get_pos_val(copy_matrix, i, j - src_matrix->col);
                    alg_matrix_set_val(*dest_matrix, i, j, val);
                }
            }
            break;

        case CONCAT_AXIS_RX: // 右侧拼接，行数相同，列数增加
            INTERATOR_MATRIX(*dest_matrix, i, j) {
                // 判断是否在目标矩阵的列范围外
                if (j >= copy_matrix->col) {
                    // 如果超出目标矩阵的列范围，从源矩阵获取值，位置偏移目标矩阵的列数
                    const alg_val_type val = *alg_matrix_get_pos_val(src_matrix, i, j - copy_matrix->col);
                    alg_matrix_set_val((*dest_matrix), i, j, val);
                } else {
                    // 否则，保持原位置，从目标矩阵获取值
                    const alg_val_type val = *alg_matrix_get_pos_val(copy_matrix, i, j);
                    alg_matrix_set_val((*dest_matrix), i, j, val);
                }
            }
            break;

        case CONCAT_AXIS_UY: // 上侧拼接，列数相同，行数增加
            INTERATOR_MATRIX(*dest_matrix, i, j) {
                // 判断是否在源矩阵的行范围内
                if (i < src_matrix->row) {
                    // 如果在源矩阵行范围内，从源矩阵获取值
                    const alg_val_type val = *alg_matrix_get_pos_val(src_matrix, i, j);
                    alg_matrix_set_val(*dest_matrix, i, j, val);
                } else {
                    // 否则，从目标矩阵获取值，位置偏移源矩阵的行数
                    const alg_val_type val = *alg_matrix_get_pos_val(copy_matrix, i - src_matrix->row, j);
                    alg_matrix_set_val(*dest_matrix, i, j, val);
                }
            }
            break;

        case CONCAT_AXIS_DY: // 下侧拼接，列数相同，行数增加
            INTERATOR_MATRIX(*dest_matrix, i, j) {
                // 判断是否在目标矩阵的行范围外
                if (i >= copy_matrix->row) {
                    // 如果超出目标矩阵的行范围，从源矩阵获取值，位置偏移目标矩阵的行数
                    const alg_val_type val = *alg_matrix_get_pos_val(src_matrix, i - copy_matrix->row, j);
                    alg_matrix_set_val((*dest_matrix), i, j, val);
                } else {
                    // 否则，保持原位置，从目标矩阵获取值
                    const alg_val_type val = *alg_matrix_get_pos_val(copy_matrix, i, j);
                    alg_matrix_set_val((*dest_matrix), i, j, val);
                }
            }
            break;
    }
    alg_matrix_free(copy_matrix);
    return ALG_OK;
}
