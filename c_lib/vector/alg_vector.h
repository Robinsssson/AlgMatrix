#ifndef __ALG_VECTOR_H__
#define __ALG_VECTOR_H__
#include "../alg_inc.h"

#define ALG_VECTOR_BASE_SIZE (32)
alg_vector *alg_vector_create(int size, alg_val_type init_val);
alg_val_type *alg_vector_get_val(alg_vector *vec, int pos);
alg_state alg_vector_set_val(alg_vector *vec, int pos, alg_val_type val);
alg_state alg_vector_insert(alg_vector *vec, int pos, alg_val_type val);
alg_state alg_vector_free(alg_vector *vec);
alg_vector *alg_vector_from_matrix_row(const alg_matrix *matrix, int row);
alg_vector *alg_vector_from_matrix_col(const alg_matrix *matrix, int col);
#endif
