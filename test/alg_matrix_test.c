#include "algmath.h"
#include "test_framework.h"
#include <stdio.h>

// Test: Create a matrix with given rows and columns
int test_alg_matrix_create() {
    alg_matrix *mat = alg_matrix_create(3, 3);
    if (mat == NULL)
        return TEST_FAILED;

    // Verify the matrix has the correct size (assuming the matrix structure has row and col
    // attributes)
    if (mat->row != 3 || mat->col != 3) {
        alg_matrix_free(mat);
        return TEST_FAILED;
    }

    alg_matrix_free(mat);
    return TEST_PASSED;
}

// Test: Set and get matrix value at specific row and column
int test_alg_matrix_set_and_get_val() {
    alg_matrix *mat = alg_matrix_create(3, 3);
    if (mat == NULL)
        return TEST_FAILED;

    if (alg_matrix_set_val(mat, 1, 1, 10.0f) != ALG_OK
        || *alg_matrix_get_pos_val(mat, 1, 1) != 10.0f) {
        alg_matrix_free(mat);
        return TEST_FAILED;
    }

    alg_matrix_free(mat);
    return TEST_PASSED;
}

// Test: Get value from matrix using linear index
int test_alg_matrix_get_index_val() {
    alg_matrix *mat = alg_matrix_create(3, 3);
    if (mat == NULL)
        return TEST_FAILED;

    alg_matrix_set_val(mat, 0, 0, 1.0f);
    alg_matrix_set_val(mat, 0, 1, 2.0f);
    alg_matrix_set_val(mat, 0, 2, 3.0f);

    if (*alg_matrix_get_index_val(mat, 0) != 1.0f || *alg_matrix_get_index_val(mat, 1) != 2.0f
        || *alg_matrix_get_index_val(mat, 2) != 3.0f) {
        alg_matrix_free(mat);
        return TEST_FAILED;
    }

    alg_matrix_free(mat);
    return TEST_PASSED;
}

// Test: Matrix addition
int test_alg_matrix_add() {
    alg_matrix *mat1 = alg_matrix_create(2, 2);
    alg_matrix *mat2 = alg_matrix_create(2, 2);

    if (mat1 == NULL || mat2 == NULL)
        return TEST_FAILED;

    alg_matrix_set_val(mat1, 0, 0, 1.0f);
    alg_matrix_set_val(mat1, 0, 1, 2.0f);
    alg_matrix_set_val(mat1, 1, 0, 3.0f);
    alg_matrix_set_val(mat1, 1, 1, 4.0f);

    alg_matrix_set_val(mat2, 0, 0, 5.0f);
    alg_matrix_set_val(mat2, 0, 1, 6.0f);
    alg_matrix_set_val(mat2, 1, 0, 7.0f);
    alg_matrix_set_val(mat2, 1, 1, 8.0f);

    alg_matrix *result = alg_matrix_add(mat1, mat2);
    if (result == NULL) {
        alg_matrix_free(mat1);
        alg_matrix_free(mat2);
        return TEST_FAILED;
    }

    if (*alg_matrix_get_pos_val(result, 0, 0) != 6.0f
        || *alg_matrix_get_pos_val(result, 0, 1) != 8.0f
        || *alg_matrix_get_pos_val(result, 1, 0) != 10.0f
        || *alg_matrix_get_pos_val(result, 1, 1) != 12.0f) {
        alg_matrix_free(mat1);
        alg_matrix_free(mat2);
        alg_matrix_free(result);
        return TEST_FAILED;
    }

    alg_matrix_free(mat1);
    alg_matrix_free(mat2);
    alg_matrix_free(result);
    return TEST_PASSED;
}

// Test: Matrix subtraction
int test_alg_matrix_subtraction() {
    alg_matrix *mat1 = alg_matrix_create(2, 2);
    alg_matrix *mat2 = alg_matrix_create(2, 2);

    if (mat1 == NULL || mat2 == NULL)
        return TEST_FAILED;

    alg_matrix_set_val(mat1, 0, 0, 5.0f);
    alg_matrix_set_val(mat1, 0, 1, 6.0f);
    alg_matrix_set_val(mat1, 1, 0, 7.0f);
    alg_matrix_set_val(mat1, 1, 1, 8.0f);

    alg_matrix_set_val(mat2, 0, 0, 1.0f);
    alg_matrix_set_val(mat2, 0, 1, 2.0f);
    alg_matrix_set_val(mat2, 1, 0, 3.0f);
    alg_matrix_set_val(mat2, 1, 1, 4.0f);

    alg_matrix *result = alg_matrix_subtraction(mat1, mat2);
    if (result == NULL) {
        alg_matrix_free(mat1);
        alg_matrix_free(mat2);
        return TEST_FAILED;
    }

    if (*alg_matrix_get_pos_val(result, 0, 0) != 4.0f
        || *alg_matrix_get_pos_val(result, 0, 1) != 4.0f
        || *alg_matrix_get_pos_val(result, 1, 0) != 4.0f
        || *alg_matrix_get_pos_val(result, 1, 1) != 4.0f) {
        alg_matrix_free(mat1);
        alg_matrix_free(mat2);
        alg_matrix_free(result);
        return TEST_FAILED;
    }

    alg_matrix_free(mat1);
    alg_matrix_free(mat2);
    alg_matrix_free(result);
    return TEST_PASSED;
}

// Test: Matrix multiplication by scalar
int test_alg_matrix_dot_number() {
    alg_matrix *mat = alg_matrix_create(2, 2);
    if (mat == NULL)
        return TEST_FAILED;

    alg_matrix_set_val(mat, 0, 0, 1.0f);
    alg_matrix_set_val(mat, 0, 1, 2.0f);
    alg_matrix_set_val(mat, 1, 0, 3.0f);
    alg_matrix_set_val(mat, 1, 1, 4.0f);

    alg_matrix *result = alg_matrix_dot_number(mat, 2.0);
    if (result == NULL) {
        alg_matrix_free(mat);
        return TEST_FAILED;
    }

    if (*alg_matrix_get_pos_val(result, 0, 0) != 2.0f
        || *alg_matrix_get_pos_val(result, 0, 1) != 4.0f
        || *alg_matrix_get_pos_val(result, 1, 0) != 6.0f
        || *alg_matrix_get_pos_val(result, 1, 1) != 8.0f) {
        alg_matrix_free(mat);
        alg_matrix_free(result);
        return TEST_FAILED;
    }

    alg_matrix_free(mat);
    alg_matrix_free(result);
    return TEST_PASSED;
}

// Test: Matrix transpose
int test_alg_matrix_transpose() {
    alg_matrix *mat = alg_matrix_create(2, 3);
    if (mat == NULL)
        return TEST_FAILED;

    alg_matrix_set_val(mat, 0, 0, 1.0f); //[1, 2, 3; 4, 5, 6] => [1, 4;2, 5; 3, 6]
    alg_matrix_set_val(mat, 0, 1, 2.0f);
    alg_matrix_set_val(mat, 0, 2, 3.0f);
    alg_matrix_set_val(mat, 1, 0, 4.0f);
    alg_matrix_set_val(mat, 1, 1, 5.0f);
    alg_matrix_set_val(mat, 1, 2, 6.0f);

    alg_matrix *transposed_mat = alg_matrix_transpose(mat);
    if (transposed_mat == NULL) {
        alg_matrix_free(mat);
        return TEST_FAILED;
    }

    if (*alg_matrix_get_pos_val(transposed_mat, 0, 0) != 1.0f
        || *alg_matrix_get_pos_val(transposed_mat, 0, 1) != 4.0f
        || *alg_matrix_get_pos_val(transposed_mat, 1, 0) != 2.0f
        || *alg_matrix_get_pos_val(transposed_mat, 1, 1) != 5.0f
        || *alg_matrix_get_pos_val(transposed_mat, 2, 0) != 3.0f
        || *alg_matrix_get_pos_val(transposed_mat, 2, 1) != 6.0f) {
        alg_matrix_free(mat);
        alg_matrix_free(transposed_mat);
        return TEST_FAILED;
    }

    alg_matrix_free(mat);
    alg_matrix_free(transposed_mat);
    return TEST_PASSED;
}
#define STR(n) str##n
#define __PRINT(matrix, str)                                                                       \
    do {                                                                                           \
        char *str = alg_matrix_print_str(matrix);                                                  \
        printf(#matrix " is %s", str);                                                             \
        ALG_FREE(str);                                                                             \
    } while (0)

// 改进后的版本，避免参数冲突并且增强可读性
#define PRINT(matrix) print_matrix(matrix, #matrix)

void print_matrix(alg_matrix *matrix, char strname[]) {
    char *str = alg_matrix_print_str(matrix); // 获取矩阵的字符串表示
    if (str != NULL) {
        printf("%s is %s", strname, str);
        ALG_FREE(str); // 假设算法库使用 ALG_FREE 来释放内存
    }
}

int test_alg_matrix_concat() {
    // 创建两个矩阵
    alg_matrix *mat1 = alg_matrix_create(2, 3);
    if (mat1 == NULL)
        return TEST_FAILED;
    alg_matrix_set_val(mat1, 0, 0, 1.0f);
    alg_matrix_set_val(mat1, 0, 1, 2.0f);
    alg_matrix_set_val(mat1, 0, 2, 3.0f);
    alg_matrix_set_val(mat1, 1, 0, 4.0f);
    alg_matrix_set_val(mat1, 1, 1, 5.0f);
    alg_matrix_set_val(mat1, 1, 2, 6.0f);

    PRINT(mat1);

    alg_matrix *mat2 = alg_matrix_create(2, 2);
    if (mat2 == NULL) {
        alg_matrix_free(mat1);
        return TEST_FAILED;
    }
    alg_matrix_set_val(mat2, 0, 0, 7.0f);
    alg_matrix_set_val(mat2, 0, 1, 8.0f);
    alg_matrix_set_val(mat2, 1, 0, 9.0f);
    alg_matrix_set_val(mat2, 1, 1, 10.0f);
    PRINT(mat2);
    // 测试左拼接（CONCAT_AXIS_LX）
    alg_state ret = alg_matrix_concat(&mat1, mat2, CONCAT_AXIS_LX);
    LOGGING("RET of LEFT is %d", ret);
    PRINT(mat1);
    if (*alg_matrix_get_pos_val(mat1, 0, 0) != 7.0f
        || *alg_matrix_get_pos_val(mat1, 0, 1) != 8.0f
        || *alg_matrix_get_pos_val(mat1, 0, 2) != 1.0f
        || *alg_matrix_get_pos_val(mat1, 0, 3) != 2.0f
        || *alg_matrix_get_pos_val(mat1, 0, 4) != 3.0f
        || *alg_matrix_get_pos_val(mat1, 1, 0) != 9.0f
        || *alg_matrix_get_pos_val(mat1, 1, 1) != 10.0f
        || *alg_matrix_get_pos_val(mat1, 1, 2) != 4.0f
        || *alg_matrix_get_pos_val(mat1, 1, 3) != 5.0f
        || *alg_matrix_get_pos_val(mat1, 1, 4) != 6.0f) {
        alg_matrix_free(mat1);
        alg_matrix_free(mat2);
        return TEST_FAILED;
    }
    alg_matrix_free(mat1);
    alg_matrix_free(mat2);
    return TEST_PASSED;
}

// Test: Free matrix memory
int test_alg_matrix_free() {
    alg_matrix *mat = alg_matrix_create(3, 3);
    if (mat == NULL)
        return TEST_FAILED;

    alg_matrix_free(mat);
    return TEST_PASSED;
}

// Main function to run all tests
int TEST_MAIN() {
    TEST_SCOPE_NEGIN = {
        INSERT_TEST(test_alg_matrix_create),        INSERT_TEST(test_alg_matrix_add),
        INSERT_TEST(test_alg_matrix_dot_number),    INSERT_TEST(test_alg_matrix_free),
        INSERT_TEST(test_alg_matrix_get_index_val), INSERT_TEST(test_alg_matrix_set_and_get_val),
        INSERT_TEST(test_alg_matrix_subtraction),   INSERT_TEST(test_alg_matrix_transpose),
        INSERT_TEST(test_alg_matrix_concat)};
    TEST_SCOPE_END
}