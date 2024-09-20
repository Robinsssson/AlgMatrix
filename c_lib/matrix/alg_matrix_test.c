#include "alg_matrix.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
    alg_matrix* mat = alg_matrix_create(3, 3);
    char* strs = alg_matrix_print_str(mat);
    printf("%s",strs);
    int k = 0;
    for (int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++) {
            alg_matrix_set_val(mat, i, j, ++k);
        }
    }
    free(strs);
    strs = alg_matrix_print_str(mat);
    printf("%s",strs);
    free(strs);
    alg_matrix_free(mat);
}