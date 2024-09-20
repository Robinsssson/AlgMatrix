#ifndef __ALG_MATRIX__H__
#define __ALG_MATRIX__H__
#include "../alg_inc.h"
#include "../vector/alg_vector.h"


typedef struct __alg_matrix alg_matrix;
// 矩阵基本操作
alg_matrix *alg_matrix_create(int row, int col);                                      // 创建矩阵
alg_state alg_matrix_set_val(alg_matrix *matrix, int row, int col, alg_val_type val); // 设置矩阵值
alg_val_type *alg_matrix_get_pos_val(const alg_matrix *matrix, int row,
                                     int col);                               // 获取某行某列的值
alg_val_type *alg_matrix_get_index_val(const alg_matrix *matrix, int index); // 根据一维索引获取值
char *alg_matrix_print_str(alg_matrix *matrix); // 将矩阵转为可打印的字符串
alg_state alg_matrix_free(alg_matrix *matrix);  // 释放矩阵
alg_matrix *alg_matrix_copy(const alg_matrix *matrix);

// 矩阵与向量
alg_matrix *alg_matrix_from_vector(const alg_vector *vec); // 从向量创建矩阵

// 矩阵运算
alg_matrix *alg_matrix_add(const alg_matrix *mat1, const alg_matrix *mat2);   // 矩阵加法
alg_matrix *alg_matrix_dot(const alg_matrix *mat1, const alg_matrix *mat2);   // 矩阵点积
alg_matrix *alg_matrix_times(const alg_matrix *mat1, const alg_matrix *mat2); // 矩阵乘法
alg_state alg_matrix_add_inplace(alg_matrix *mat1, const alg_matrix *mat2);   // 就地矩阵加法
alg_state alg_matrix_dot_inplace(alg_matrix *mat1, const alg_matrix *mat2);   // 就地点积

// 矩阵扩展操作
alg_matrix *alg_matrix_transpose(const alg_matrix *mat); // 矩阵转置
alg_state alg_matrix_transpose_inplace(alg_matrix *mat); // 矩阵转置

#endif // __ALG_MATRIX__H__
