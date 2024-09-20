#ifndef __ALG_MATRIX__H__
#define __ALG_MATRIX__H__

typedef double alg_matrix_type;
#define USE_ASSERT 1
typedef struct {
    int row;
    int col;
    alg_matrix_type *mat;
} alg_matrix;
typedef enum {
    ALG_MATRIX_OK,
    ALG_MATRIX_ERROR,
} alg_matrix_state;

alg_matrix* alg_matrix_create(int row, int col);
alg_matrix_state alg_matrix_set_val(alg_matrix *matrix, int row, int col, alg_matrix_type val);
alg_matrix_type* alg_matrix_get_pos_val(alg_matrix* matrix, int row, int col);
alg_matrix_type* alg_matrix_get_index_val(alg_matrix* matrix, int index);
char* alg_matrix_print_str(alg_matrix *matrix);
alg_matrix_state alg_matrix_free(alg_matrix *matrix);


alg_matrix* alg_matrix_add(const alg_matrix* mat1, const alg_matrix* mat2);
alg_matrix* alg_matrix_dot(const alg_matrix* mat1, const alg_matrix* mat2);
alg_matrix* alg_matrix_times(const alg_matrix* mat1, const alg_matrix* mat2);
alg_matrix_state alg_matrix_add_(alg_matrix* mat1, const alg_matrix* mat2);
alg_matrix_state alg_matrix_dot_(alg_matrix* mat1, const alg_matrix* mat2);
alg_matrix_state alg_matrix_times_(alg_matrix* mat1, const alg_matrix* mat2);

#endif  //!__ALG_MATRIX__H__