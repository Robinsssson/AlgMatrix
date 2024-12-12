#include "algmath.h"
#include "test_framework.h"
#include <math.h>
#include <stdio.h>

// Test: Create a vector with given size and initial value
static int test_alg_vector_create(void) {
    const int test_number = 500;
    TESTLOG("START TEST INIT CREATE API");
    alg_vector *vec = alg_vector_create(10, 5.0f);
    if (vec == NULL)
        return TEST_FAILED;
    for (int i = 0; i < vec->size; i++) {
        if (fabs(*alg_vector_get_val(vec, i) - 5.0f) > 1e-9) {
            alg_vector_free(vec);
            return TEST_FAILED;
        }
    }
    alg_vector_free(vec);
    TESTLOG("START TEST PRESS OF CREATE");
    alg_vector *vecs[test_number];
    for (int i = 0; i < test_number; i++) {
        alg_vector *new_vecs = alg_vector_create(1000, 5.0f);
        if (new_vecs == NULL)
            return TEST_FAILED;
        vecs[i] = new_vecs;
    }
    TESTLOG("OVER TEST PRESS OF CREATE");
    if (fabs(*alg_vector_get_val(vecs[test_number / 100], 700) - 5.0f) > 1e-9)
        return TEST_FAILED;
    TESTLOG("START FREE");
    for (int i = 0; i < test_number; i++) {
        alg_vector_free(vecs[i]);
    }
    return TEST_PASSED;
}

// Test: Get value from a vector at a specific position
static int test_alg_vector_get_val(void) {
    alg_vector *vec = alg_vector_create(5, 10.0f);
    if (vec == NULL)
        return TEST_FAILED;

    for (int i = 0; i < 5; i++) {
        if (fabs(*alg_vector_get_val(vec, i) - 10.0f) > 1e-9) {
            alg_vector_free(vec);
            return TEST_FAILED;
        }
    }
    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Set value at a specific position in the vector
static int test_alg_vector_set_val(void) {
    alg_vector *vec = alg_vector_create(5, 1.0f);
    if (vec == NULL)
        return TEST_FAILED;

    if (alg_vector_set_val(vec, 2, 3.0f) != ALG_OK || fabs(*alg_vector_get_val(vec, 2) - 3.0f) > 1e-9) {
        alg_vector_free(vec);
        return TEST_FAILED;
    }
    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Insert value at a specific position in the vector
static int test_alg_vector_insert(void) {
    alg_vector *vec = alg_vector_create(5, 10.0f);
    if (vec == NULL)
        return TEST_FAILED;

    TESTLOG("START LARGE NUMBER TEST");
    for (int i = 0; i < 160000; i++) {
        if (alg_vector_insert(&vec, 0, 8.f) == ALG_ERROR)
            return TEST_FAILED;
    }

    TESTLOG("START LARGE NUMBER SIZE ENSURE TEST");
    if (vec->size != 160005)
        return TEST_FAILED;

    TESTLOG("START LARGE NUMBER ENSURE TEST");
    if (fabs(*alg_vector_get_val(vec, 160003) - 10.0f) > 1e-9) {
        alg_vector_free(vec);
        return TEST_FAILED;
    }

    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Free the vector
static int test_alg_vector_free(void) {
    alg_vector *vec = alg_vector_create(5, 1.0f);
    if (vec == NULL)
        return TEST_FAILED;

    alg_vector_free(vec);
    return TEST_PASSED;
}

// Test: Sort vector (copy) with a custom comparator
static int test_alg_vector_sort_copy(void) {
    const int test_number = 500;
    alg_vector *vec = alg_vector_create(test_number, 1.0f);
    for (int i = 0; i < test_number; i++) {
        alg_vector_set_val(vec, i, test_number - i);
    } // {5, 4, 3, 2, 1}
    TESTLOG("START SORTED");
    alg_vector *sorted_vec = alg_vector_create(test_number, 0.0f);
    int sort_index[test_number];
    if (alg_vector_sort_copy(sorted_vec, vec, sort_index, alg_utils_greater) != ALG_OK) {
        alg_vector_free(vec);
        alg_vector_free(sorted_vec);
        return TEST_FAILED;
    }
    TESTLOG("IDENTIFY THE SORT API");
    for (int i = 0; i < test_number; i++) {
        if ((int)*alg_vector_get_val(sorted_vec, i) != i + 1 && sort_index[i] != test_number - i) {
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
static int test_alg_vector_slice(void) {
    const int test_number = 50000;
    alg_vector *vec = alg_vector_create(test_number, 1.0f);
    for (int i = 0; i < test_number; i++) {
        alg_vector_set_val(vec, i, i);
    }
    alg_vector *sliced_vec;
    TESTLOG("LEFT SLICE BEGIN");
    sliced_vec = alg_vector_slice(vec, ALG_ALL_RANGE, test_number / 100);
    if (sliced_vec == NULL)
        return TEST_FAILED;
    if (sliced_vec->size != test_number / 100) {
        return TEST_FAILED;
    }
    TESTLOG_ARGS("EXPECT NUMBER %f, GET %f", (double)test_number / 100 - 5, sliced_vec->vector[test_number / 100 - 5]);
    if (fabs(sliced_vec->vector[test_number / 100 - 5] - (double)test_number / 100 + 5) > 1e-9)
        return TEST_FAILED;
    alg_vector_free(sliced_vec);

    TESTLOG("RIGHT SLICE BEGIN");
    sliced_vec = alg_vector_slice(vec, test_number - test_number / 100, ALG_ALL_RANGE);
    if (sliced_vec == NULL)
        return TEST_FAILED;
    if (sliced_vec->size != test_number / 100) {
        return TEST_FAILED;
    }
    TESTLOG_ARGS("EXPECT NUMBER %f, GET %f", (double)test_number - 5, sliced_vec->vector[test_number / 100 - 5]);
    if (fabs(sliced_vec->vector[test_number / 100 - 5] - test_number + 5) > 1e-9)
        return TEST_FAILED;
    alg_vector_free(sliced_vec);

    TESTLOG("MID SLICE BEGIN");
    int a = alg_random_int(0, test_number / 2);
    int b = alg_random_int(test_number / 2, test_number);
    sliced_vec = alg_vector_slice(vec, a, b);
    TESTLOG_ARGS("EXPECT NUMBER %d, %d, GET %d", a, b, sliced_vec->size);
    if (sliced_vec->size != (b - a)) {
        return TEST_FAILED;
    }
    alg_vector_free(vec);
    alg_vector_free(sliced_vec);
    return TEST_PASSED;
}
// Test: Concatenate two vectors (right and left concat)
static int test_alg_vector_concat_inplace(void) {
    // Create two vectors
    const int test_val1 = 30;
    const int test_val2 = 40;
    alg_vector *vec1 = alg_vector_create(test_val1, 1.0f); // {1.0f, 1.0f, 1.0f}
    alg_vector *vec2 = alg_vector_create(test_val2, 2.0f); // {2.0f, 2.0f}
    if (vec1 == NULL || vec2 == NULL)
        return TEST_FAILED;
    TESTLOG("CONCAT RIGHT START");
    // Perform right concatenation (vec1 + vec2)
    alg_vector_concat_inplace(&vec1, vec2, ALG_VECTOR_CONCAT_R);
    // Check the size of the concatenated vector
    if (vec1->size != test_val1 + test_val2) {
        alg_vector_free(vec1);
        alg_vector_free(vec2);
        return TEST_FAILED;
    }
    TESTLOG("IDENTIFY CONCAT RIGHT START");
    // Check the values after right concatenation: {1.0f, 1.0f, 1.0f, 2.0f, 2.0f}
    for (int i = 0; i < test_val1 + test_val2; i++) {
        alg_val_type identify = i < test_val1 ? 1.0f : 2.0f;
        if (!FLOAT_COMPARE_IS(vec1->vector[i], identify))
            return TEST_FAILED;
    }
    TESTLOG("CONCAT RIGHT OVER");

    // Perform left concatenation (vec1 + vec2), so vec1 will have the values from vec2 prepended
    alg_vector_concat_inplace(&vec1, vec2, ALG_VECTOR_CONCAT_L);
    TESTLOG("CONCAT LEFT START");
    // Check the size of the concatenated vector
    if (vec1->size != 2 * test_val2 + test_val1) {
        alg_vector_free(vec1);
        alg_vector_free(vec2);
        return TEST_FAILED;
    }
    TESTLOG("IDENTIFY CONCAT LEFT START");
    for (int i = 0; i < test_val1 + 2 * test_val2; i++) {
        alg_val_type identify = i < test_val2 || i >= test_val1 + test_val2 ? 2.0f : 1.0f;
        if (!FLOAT_COMPARE_IS(vec1->vector[i], identify))
            return TEST_FAILED;
    }
    TESTLOG("CONCAT OVER START");
    // Free the vectors
    alg_vector_free(vec1);
    alg_vector_free(vec2);

    return TEST_PASSED;
}

// Main function to run all tests
int TEST_MAIN(void) {
    TEST_SCOPE_NEGIN = {INSERT_TEST(test_alg_vector_create),  INSERT_TEST(test_alg_vector_get_val),
                        INSERT_TEST(test_alg_vector_set_val), INSERT_TEST(test_alg_vector_insert),
                        INSERT_TEST(test_alg_vector_free),    INSERT_TEST(test_alg_vector_sort_copy),
                        INSERT_TEST(test_alg_vector_slice),   INSERT_TEST(test_alg_vector_concat_inplace)};
    TEST_SCOPE_END
}
