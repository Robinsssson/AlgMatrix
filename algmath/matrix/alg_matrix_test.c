#include "../algmath.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
    alg_matrix *mat1 = alg_matrix_create(3, 3);
    alg_matrix *mat2 = alg_matrix_create(3, 3);
    char *strs = alg_matrix_print_str(mat1);
    printf("%s\n", strs);
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            alg_matrix_set_val(mat1, i, j, ++k);
        }
    }
    k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            alg_matrix_set_val(mat2, i, j, ++k);
        }
    }
    free(strs);
    strs = alg_matrix_print_str(mat1);
    printf("%s\n", strs);
    free(strs);
    alg_matrix *ret = alg_matrix_times(mat1, mat2);
    strs = alg_matrix_print_str(ret);
    printf("%s\n", strs);
    free(strs);
    alg_matrix_free(mat1);
    alg_matrix_free(mat2);
    alg_matrix_free(ret);
}