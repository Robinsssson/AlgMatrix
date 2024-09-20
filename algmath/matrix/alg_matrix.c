#include "alg_matrix.h"
#include <time.h>

alg_matrix *alg_matrix_create(int row, int col) {
    alg_matrix *mat = (alg_matrix *)malloc(sizeof(alg_matrix));
    if (mat == NULL)
        return NULL;

    mat->col = col;
    mat->row = row;
    mat->mat = (alg_val_type *)calloc(row * col, sizeof(alg_val_type)); // 分配正确大小的内存
    if (mat->mat == NULL) {
        free(mat);
        return NULL;
    }
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

alg_val_type *alg_matrix_get_pos_val(const alg_matrix *matrix, int row, int col) {
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

alg_val_type *alg_matrix_get_index_val(const alg_matrix *matrix, int index) {
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
    memcpy(ret->mat, matrix->mat, sizeof(alg_val_type) * matrix->col * matrix->row);
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

alg_matrix *alg_matrix_times(const alg_matrix *mat1, const alg_matrix *mat2) {
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
            alg_val_type *val = alg_matrix_get_pos_val(ret, i, j);
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
    memcpy(matrix->mat, vec->vector, vec->size * sizeof(alg_val_type));
    return matrix;
}

char *alg_matrix_print_str(alg_matrix *matrix) {
    if (matrix == NULL || matrix->mat == NULL) {
        return NULL;
    }

    // 估算初始缓冲区大小。每个数字最多保留 2 位小数，含符号、逗号、空格等。
    int buf_size = matrix->row * matrix->col * 25 + matrix->row * 4 + 4; // 初始估算
    char *str = (char *)malloc(buf_size);
    if (str == NULL) {
        return NULL;
    }

    int pos = 0;
    pos += snprintf(str + pos, buf_size - pos, "[\n"); // 开始矩阵输出

    for (int i = 0; i < matrix->row; i++) {
        pos += snprintf(str + pos, buf_size - pos, " ["); // 每行的起始方括号
        for (int j = 0; j < matrix->col; j++) {
            // 每个元素以 "%.2f" 格式输出，后跟逗号
            pos += snprintf(str + pos, buf_size - pos, "%.2f", matrix->mat[i * matrix->col + j]);
            if (j < matrix->col - 1) {
                pos += snprintf(str + pos, buf_size - pos, ", ");
            }
        }
        pos += snprintf(str + pos, buf_size - pos, "]"); // 行的结束方括号
        if (i < matrix->row - 1) {
            pos += snprintf(str + pos, buf_size - pos, ",\n"); // 行之间的逗号换行
        }
    }
    pos += snprintf(str + pos, buf_size - pos, "\n]"); // 矩阵结束方括号

    return str;
}

alg_state alg_matrix_free(alg_matrix *matrix) {
    if (matrix == NULL)
        return ALG_ERROR;

    if (matrix->mat != NULL) {
        free(matrix->mat);
    }
    free(matrix);
    return ALG_OK;
}

alg_matrix *alg_matrix_transpose(const alg_matrix *mat) {
    alg_matrix *ret = alg_matrix_copy(mat);
    int tmp = ret->col;
    ret->col = ret->row;
    ret->row = tmp;
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
};

void alg_matrix_fill_random(alg_matrix *matrix, double min_val, double max_val) {
    // 如果矩阵为空，直接返回
    if (matrix == NULL || matrix->mat == NULL)
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
    if (matrix == NULL || matrix->mat == NULL)
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
