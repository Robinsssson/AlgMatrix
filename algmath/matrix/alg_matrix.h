#ifndef __ALG_MATRIX__H__
#define __ALG_MATRIX__H__
#include "../alg_inc.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct __alg_matrix alg_matrix;
// 矩阵基本操作
/**
 * @brief 创建一个矩阵对象
 *
 * @param row 矩阵的行数
 * @param col 矩阵的列数
 * @return 返回指向新创建矩阵的指针，如果创建失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_create(int row, int col);

/**
 * @brief 为矩阵对象赋值
 *
 * @param matrix 指向要设置值的矩阵对象
 * @param row 行索引
 * @param col 列索引
 * @param val 要设置的值
 * @return 返回状态，成功则为 ALG_OK，失败则为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_matrix_set_val(alg_matrix *matrix, int row, int col, alg_val_type val);

/**
 * @brief 获取矩阵指定位置的值的指针
 *
 * @param matrix 指向矩阵对象的指针
 * @param row 行索引
 * @param col 列索引
 * @return 返回指定位置的值的指针，如果索引越界则返回 NULL
 */
ALG_MATH_API alg_val_type *alg_matrix_get_pos_val(alg_matrix *matrix, int row, int col);

/**
 * @brief 获取矩阵指定索引的值的指针
 *
 * @param matrix 指向矩阵对象的指针
 * @param index 元素的线性索引
 * @return 返回指定索引的值的指针，如果索引越界则返回 NULL
 */
ALG_MATH_API alg_val_type *alg_matrix_get_index_val(alg_matrix *matrix, int index);

/**
 * @brief 复制矩阵
 *
 * @param matrix 指向要复制的矩阵对象的指针
 * @return 返回指向新复制矩阵的指针，如果复制失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_copy(const alg_matrix *matrix);

/**
 * @brief 矩阵加法
 *
 * @param mat1 指向第一个加数矩阵的指针
 * @param mat2 指向第二个加数矩阵的指针
 * @return 返回指向结果矩阵的指针，如果加法失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_add(const alg_matrix *mat1, const alg_matrix *mat2);

/**
 * @brief 矩阵减法
 *
 * @param mat1 指向被减数矩阵的指针
 * @param mat2 指向减数矩阵的指针
 * @return 返回指向结果矩阵的指针，如果减法失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_subtraction(const alg_matrix *mat1, const alg_matrix *mat2);

/**
 * @brief 矩阵点积
 *
 * @param mat1 指向第一个点积矩阵的指针
 * @param mat2 指向第二个点积矩阵的指针
 * @return 返回指向结果矩阵的指针，如果点积失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_dot(const alg_matrix *mat1, const alg_matrix *mat2);

/**
 * @brief 矩阵与标量相乘
 *
 * @param mat1 指向矩阵的指针
 * @param number 要乘的标量
 * @return 返回指向结果矩阵的指针，如果乘法失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_dot_number(const alg_matrix *mat1, const double number);

/**
 * @brief 矩阵乘法
 *
 * @param mat1 指向第一个矩阵的指针
 * @param mat2 指向第二个矩阵的指针
 * @return 返回指向结果矩阵的指针，如果乘法失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_times(alg_matrix *mat1, alg_matrix *mat2);

/**
 * @brief 就地矩阵加法
 *
 * @param mat1 指向要加法的第一个矩阵的指针
 * @param mat2 指向要加法的第二个矩阵的指针
 * @return 返回状态，成功则为 ALG_OK，失败则为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_matrix_add_inplace(alg_matrix *mat1, const alg_matrix *mat2);

/**
 * @brief 就地矩阵点积
 *
 * @param mat1 指向要进行点积的第一个矩阵的指针
 * @param mat2 指向要进行点积的第二个矩阵的指针
 * @return 返回状态，成功则为 ALG_OK，失败则为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_matrix_dot_inplace(alg_matrix *mat1, const alg_matrix *mat2);

/**
 * @brief 就地矩阵与标量相乘
 *
 * @param mat1 指向要乘的矩阵的指针
 * @param number 要乘的标量
 * @return 返回状态，成功则为 ALG_OK，失败则为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_matrix_dot_number_inplace(alg_matrix *mat1, const double number);

/**
 * @brief 从向量创建矩阵
 *
 * @param vec 指向向量对象的指针
 * @return 返回指向新创建矩阵的指针，如果创建失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_from_vector(const alg_vector *vec);

/**
 * @brief 打印矩阵为字符串
 *
 * @param matrix 指向要打印的矩阵的指针
 * @return 返回表示矩阵的字符串，如果打印失败则返回 NULL
 */
ALG_MATH_API char *alg_matrix_print_str(alg_matrix *matrix);

/**
 * @brief 释放矩阵内存
 *
 * @param matrix 指向要释放的矩阵的指针
 * @return 返回状态，成功则为 ALG_OK，失败则为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_matrix_free(alg_matrix *matrix);

/**
 * @brief 矩阵转置
 *
 * @param mat 指向要转置的矩阵的指针
 * @return 返回指向转置后矩阵的指针，如果转置失败则返回 NULL
 */
ALG_MATH_API alg_matrix *alg_matrix_transpose(alg_matrix *mat);

/**
 * @brief 就地矩阵转置
 *
 * @param mat 指向要转置的矩阵的指针
 * @return 返回状态，成功则为 ALG_OK，失败则为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_matrix_transpose_inplace(alg_matrix *mat);

/**
 * @brief 设置矩阵的某一行
 *
 * @param matrix 指向要设置行的矩阵对象
 * @param row 行索引
 * @param vec 指向要复制到矩阵行的向量对象
 * @return 返回状态，成功则为 ALG_OK，失败则为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_matrix_set_row(alg_matrix *matrix, int row, const alg_vector *vec);

/**
 * @brief 为矩阵填充随机值
 *
 * @param matrix 指向要填充随机值的矩阵对象
 * @param min_val 随机值的最小值
 * @param max_val 随机值的最大值
 */
ALG_MATH_API void alg_matrix_fill_random(alg_matrix *matrix, double min_val, double max_val);

/**
 * @brief 限定矩阵元素的幅值
 *
 * @param matrix 指向要限制幅值的矩阵对象
 * @param min_val 元素的最小值
 * @param max_val 元素的最大值
 */
ALG_MATH_API void alg_matrix_clamp(alg_matrix *matrix, double min_val, double max_val);

#ifdef __cplusplus
}
#endif

#endif // __ALG_MATRIX__H__
