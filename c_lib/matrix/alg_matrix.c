#include "alg_matrix.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * @brief 创建一个矩阵对象
 *
 * @param row
 * @param col
 * @return alg_matrix*
 */
alg_matrix* alg_matrix_create(int row, int col)
{
    alg_matrix* mat = (alg_matrix*)malloc(sizeof(alg_matrix));
    if (mat == NULL)
        return NULL;

    mat->col = col;
    mat->row = row;
    mat->mat = (alg_matrix_type*)calloc(row * col, sizeof(alg_matrix_type)); // 分配正确大小的内存
    if (mat->mat == NULL) {
        free(mat);
        return NULL;
    }
    return mat;
}
/**
 * @brief 为矩阵对象赋值
 *
 * @param matrix
 * @param row
 * @param col
 * @param val
 * @return alg_matrix_state
 */
alg_matrix_state alg_matrix_set_val(alg_matrix* matrix, int row, int col, alg_matrix_type val)
{
#if USE_ASSERT
    assert(matrix != NULL);
#else
    if (matrix == NULL)
        return ALG_MATRIX_ERROR;
#endif
    if (row >= matrix->row || col >= matrix->col)
        return ALG_MATRIX_ERROR; // 检查索引是否越界
    matrix->mat[row * matrix->col + col] = val;
    return ALG_MATRIX_OK;
}

alg_matrix_type* alg_matrix_get_pos_val(alg_matrix* matrix, int row, int col)
{
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

alg_matrix_type* alg_matrix_get_index_val(alg_matrix* matrix, int index)
{
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

alg_matrix* alg_matrix_add(const alg_matrix* mat1, const alg_matrix* mat2)
{
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat2->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return ALG_MATRIX_ERROR;
    if (mat1->col != mat2->col || mat2->row != mat2->row)
        return ALG_MATRIX_ERROR;
#endif
    alg_matrix* ret = alg_matrix_create(mat1->row, mat1->col);
    for (int i = 0; i < mat1->col * mat1->row; i++)
        ret->mat[i] = mat1->mat[i] + mat2->mat[i];
    return ret;
}
alg_matrix* alg_matrix_dot(const alg_matrix* mat1, const alg_matrix* mat2)
{
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat2->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return ALG_MATRIX_ERROR;
    if (mat1->col != mat2->col || mat2->row != mat2->row)
        return ALG_MATRIX_ERROR;
#endif
    alg_matrix* ret = alg_matrix_create(mat1->row, mat1->col);
    for (int i = 0; i < mat1->col * mat1->row; i++)
        ret->mat[i] = mat1->mat[i] * mat2->mat[i];
    return ret;
}
alg_matrix* alg_matrix_times(const alg_matrix* mat1, const alg_matrix* mat2);
alg_matrix_state alg_matrix_add_(alg_matrix* mat1, const alg_matrix* mat2)
{
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat2->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return ALG_MATRIX_ERROR;
    if (mat1->col != mat2->col || mat2->row != mat2->row)
        return ALG_MATRIX_ERROR;
#endif
    for (int i = 0; i < mat1->col * mat1->row; i++)
        mat1->mat[i] += mat2->mat[i];
    return ALG_MATRIX_OK;
}
alg_matrix_state alg_matrix_dot_(alg_matrix* mat1, const alg_matrix* mat2)
{
#if USE_ASSERT
    assert(mat1 != NULL && mat2 != NULL);
    assert(mat1->col == mat2->col && mat2->row == mat2->row);
#else
    if (mat1 == NULL || mat2 == NULL)
        return ALG_MATRIX_ERROR;
    if (mat1->col != mat2->col || mat2->row != mat2->row)
        return ALG_MATRIX_ERROR;
#endif
    for (int i = 0; i < mat1->col * mat1->row; i++)
        mat1->mat[i] *= mat2->mat[i];
    return ALG_MATRIX_OK;
}
alg_matrix_state alg_matrix_times_(alg_matrix* mat1, const alg_matrix* mat2);

char* alg_matrix_print_str(alg_matrix* matrix)
{
    if (matrix == NULL || matrix->mat == NULL)
        return NULL;

    int buf_size = matrix->row * matrix->col * 20; // 预估每个元素最多20个字符
    char* str = (char*)malloc(buf_size);
    if (str == NULL)
        return NULL;

    int pos = 0;
    for (int i = 0; i < matrix->row; i++) {
        for (int j = 0; j < matrix->col; j++) {
            pos += snprintf(str + pos, buf_size - pos, "%.2f\t", matrix->mat[i * matrix->col + j]);
        }
        pos += snprintf(str + pos, buf_size - pos, "\n");
    }
    return str;
}
alg_matrix_state alg_matrix_free(alg_matrix* matrix)
{
    if (matrix == NULL)
        return ALG_MATRIX_ERROR;

    if (matrix->mat != NULL) {
        free(matrix->mat);
    }
    free(matrix);
    return ALG_MATRIX_OK;
}
