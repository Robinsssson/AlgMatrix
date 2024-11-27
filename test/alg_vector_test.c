#include "algmath.h"
#include "test_framework.h"
#include <stdio.h>

// Test: Create a vector with given size and initial value
int test_alg_vector_create() {
    alg_vector *vec = alg_vector_create(10, 5.0f);
    if (vec == NULL)
        return TEST_FAILED;

    for (int i = 0; i < vec->size; i++) {
        if (*alg_vector_get_val(vec, i) != 5.0f) {
            alg_vector_free(vec);
            return TEST_FAILED;
        }
    }
    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Get value from a vector at a specific position
int test_alg_vector_get_val() {
    alg_vector *vec = alg_vector_create(5, 10.0f);
    if (vec == NULL)
        return TEST_FAILED;

    for (int i = 0; i < 5; i++) {
        if (*alg_vector_get_val(vec, i) != 10.0f) {
            alg_vector_free(vec);
            return TEST_FAILED;
        }
    }
    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Set value at a specific position in the vector
int test_alg_vector_set_val() {
    alg_vector *vec = alg_vector_create(5, 1.0f);
    if (vec == NULL)
        return TEST_FAILED;

    if (alg_vector_set_val(vec, 2, 3.0f) != ALG_OK || *alg_vector_get_val(vec, 2) != 3.0f) {
        alg_vector_free(vec);
        return TEST_FAILED;
    }
    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Insert value at a specific position in the vector
int test_alg_vector_insert() {
    alg_vector *vec = alg_vector_create(5, 10.0f);
    if (vec == NULL)
        return TEST_FAILED;

    if (alg_vector_insert(&vec, 2, 20.0f) != ALG_OK || *alg_vector_get_val(vec, 2) != 20.0f) {
        alg_vector_free(vec);
        return TEST_FAILED;
    }

    if (*alg_vector_get_val(vec, 3) != 10.0f) {
        alg_vector_free(vec);
        return TEST_FAILED;
    }

    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Free the vector
int test_alg_vector_free() {
    alg_vector *vec = alg_vector_create(5, 1.0f);
    if (vec == NULL)
        return TEST_FAILED;

    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Sort vector (copy) with a custom comparator
int test_alg_vector_sort_copy() {
    alg_vector *vec = alg_vector_create(5, 1.0f);
    for (int i = 0; i < 5; i++) {
        alg_vector_set_val(vec, i, 5 - i);
    } // {5, 4, 3, 2, 1}

    alg_vector *sorted_vec = alg_vector_create(5, 0.0f);
    int sort_index[5];
    if (alg_vector_sort_copy(vec, sorted_vec, sort_index, alg_utils_greater) != ALG_OK) {
        alg_vector_free(vec);
        alg_vector_free(sorted_vec);
        return TEST_FAILED;
    }

    for (int i = 0; i < 5; i++) {
        if ((int)*alg_vector_get_val(sorted_vec, i) != i + 1) {
            alg_vector_free(vec);
            alg_vector_free(sorted_vec);
            return TEST_FAILED;
        }
    }

    alg_vector_free(vec);
    alg_vector_free(sorted_vec);
    return TEST_PASSED;
}

// Test: Slice a vector (create a sub-vector)
int test_alg_vector_slice() {
    alg_vector *vec = alg_vector_create(5, 1.0f);
    for (int i = 0; i < 5; i++) {
        alg_vector_set_val(vec, i, i + 1);
    }

    alg_vector *sliced_vec = alg_vector_slice(vec, 1, 4);
    if (sliced_vec == NULL) {
        alg_vector_free(vec);
        return TEST_FAILED;
    }

    if (sliced_vec->size != 3 || *alg_vector_get_val(sliced_vec, 0) != 2 || *alg_vector_get_val(sliced_vec, 1) != 3
        || *alg_vector_get_val(sliced_vec, 2) != 4) {
        alg_vector_free(vec);
        alg_vector_free(sliced_vec);
        return TEST_FAILED;
    }

    alg_vector_free(vec);
    alg_vector_free(sliced_vec);
    return TEST_PASSED;
}
// Test: Concatenate two vectors (right and left concat)
int test_alg_vector_concat_inplace() {
    // Create two vectors
    alg_vector *vec1 = alg_vector_create(3, 1.0f); // {1.0f, 1.0f, 1.0f}
    alg_vector_set_val(vec1, 2, 8.0f);
    alg_vector *vec2 = alg_vector_create(2, 2.0f); // {2.0f, 2.0f}
    if (vec1 == NULL || vec2 == NULL)
        return TEST_FAILED;

    // Perform right concatenation (vec1 + vec2)
    alg_vector_concat_inplace(&vec1, vec2, ALG_VECTOR_CONCAT_R);
    LOGGING("Concat1 Over");
    // Check the size of the concatenated vector
    if (vec1->size != 5) {
        alg_vector_free(vec1);
        alg_vector_free(vec2);
        return TEST_FAILED;
    }

    // Check the values after right concatenation: {1.0f, 1.0f, 1.0f, 2.0f, 2.0f}
    if (*alg_vector_get_val(vec1, 0) != 1.0f || *alg_vector_get_val(vec1, 1) != 1.0f || *alg_vector_get_val(vec1, 2) != 8.0f
        || *alg_vector_get_val(vec1, 3) != 2.0f || *alg_vector_get_val(vec1, 4) != 2.0f) {
        alg_vector_free(vec1);
        alg_vector_free(vec2);
        return TEST_FAILED;
    }
    LOGGING("Concat1 TEST Over");
    // Perform left concatenation (vec1 + vec2), so vec1 will have the values from vec2 prepended
    alg_vector_concat_inplace(&vec1, vec2, ALG_VECTOR_CONCAT_L);
    LOGGING("Concat2 Over");
    // Check the size of the concatenated vector
    if (vec1->size != 7) {
        alg_vector_free(vec1);
        alg_vector_free(vec2);
        return TEST_FAILED;
    }

    // Check the values after left concatenation: {2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f}
    if (*alg_vector_get_val(vec1, 0) != 2.0f || *alg_vector_get_val(vec1, 1) != 2.0f || *alg_vector_get_val(vec1, 2) != 1.0f
        || *alg_vector_get_val(vec1, 3) != 1.0f || *alg_vector_get_val(vec1, 4) != 8.0f || *alg_vector_get_val(vec1, 5) != 2.0f
        || *alg_vector_get_val(vec1, 6) != 2.0f) {
        alg_vector_free(vec1);
        alg_vector_free(vec2);
        return TEST_FAILED;
    }

    // Free the vectors
    alg_vector_free(vec1);
    alg_vector_free(vec2);

    return TEST_PASSED;
}

// Main function to run all tests
int TEST_MAIN() {
    TEST_SCOPE_NEGIN = {INSERT_TEST(test_alg_vector_create), INSERT_TEST(test_alg_vector_get_val),       INSERT_TEST(test_alg_vector_set_val),
                        INSERT_TEST(test_alg_vector_insert), INSERT_TEST(test_alg_vector_free),          INSERT_TEST(test_alg_vector_sort_copy),
                        INSERT_TEST(test_alg_vector_slice),  INSERT_TEST(test_alg_vector_concat_inplace)};
    TEST_SCOPE_END
}
