#ifndef __ALG_VECTOR_H__
#define __ALG_VECTOR_H__
#include "../alg_inc.h"
#ifdef __cplusplus
extern "C" {
#endif
#define ALG_VECTOR_BASE_SIZE (32)
/**
 * @brief 创建一个向量对象
 *
 * @param size 向量的大小
 * @param init_val 向量元素的初始值
 * @return alg_vector* 指向创建的向量对象的指针，失败时返回 NULL
 */
ALG_MATH_API alg_vector *alg_vector_create(int size, alg_val_type init_val);

/**
 * @brief 获取向量指定位置的值
 *
 * @param vec 指向向量对象的指针
 * @param pos 要获取的值的位置
 * @return alg_val_type* 指向指定位置值的指针，失败时返回 NULL
 */
ALG_MATH_API alg_val_type *alg_vector_get_val(alg_vector *vec, int pos);

/**
 * @brief 设置向量指定位置的值
 *
 * @param vec 指向向量对象的指针
 * @param pos 要设置的值的位置
 * @param val 要设置的值
 * @return alg_state 返回操作状态，成功时为 ALG_OK，失败时为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_vector_set_val(alg_vector *vec, int pos, alg_val_type val);

/**
 * @brief 在向量的指定位置插入一个值
 *
 * @param vec 指向向量对象的指针
 * @param pos 要插入值的位置
 * @param val 要插入的值
 * @return alg_state 返回操作状态，成功时为 ALG_OK，失败时为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_vector_insert(alg_vector *vec, int pos, alg_val_type val);

/**
 * @brief 释放向量占用的内存
 *
 * @param vec 指向要释放的向量对象的指针
 * @return alg_state 返回操作状态，成功时为 ALG_OK，失败时为 ALG_ERROR
 */
ALG_MATH_API alg_state alg_vector_free(alg_vector *vec);

/**
 * @brief 从矩阵的指定行创建向量
 *
 * @param matrix 指向矩阵对象的指针
 * @param row 要提取的行索引
 * @return alg_vector* 指向创建的向量对象的指针，失败时返回 NULL
 */
ALG_MATH_API alg_vector *alg_vector_from_matrix_row(alg_matrix *matrix, int row);

/**
 * @brief 从矩阵的指定列创建向量
 *
 * @param matrix 指向矩阵对象的指针
 * @param col 要提取的列索引
 * @return alg_vector* 指向创建的向量对象的指针，失败时返回 NULL
 */
ALG_MATH_API alg_vector *alg_vector_from_matrix_col(alg_matrix *matrix, int col);

/**
 * @brief 打印向量的结果
 *
 * @param vector 指向向量对象的指针
 * @return char* 指向创建的向量对象的指针，失败时返回 NULL
 */
ALG_MATH_API char *alg_vector_print_str(const alg_vector *vector);
/**
 * @brief 获取vector sort的结果
 *
 * @param src_vector  指向原始向量对象的指针
 * @param dest_vector 指向排序后的向量的指针，此处内存需要手动创建手动释放
 * @param sort_index  指向排序过后的index变化值
 * @param ptr_compare 在"alg_utils.h"中定义的函数
 * @return char* 指向创建的向量对象的指针，失败时返回 NULL
 */
ALG_MATH_API alg_state alg_vector_sort_copy(const alg_vector *src_vector, alg_vector *dest_vector,
                                            int *sort_index,
                                            int (*ptr_compare)(const void *, const void *));

/**
 * @brief 输出数组切片
 *
 * @param vector 指向向量对象的指针
 * @return char* 指向创建的向量对象的指针，失败时返回 NULL
 */
ALG_MATH_API alg_vector *alg_vector_slice(const alg_vector *vector, int range_l, int range_r);
#ifdef __cplusplus
}
#endif
#endif
