#include "alg_vector.h"
#include "../matrix/alg_matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

alg_vector *alg_vector_create(int size, alg_val_type init_val) {
    alg_vector *vec = ALG_MALLOC(sizeof(alg_vector));
    if (vec == NULL)
        return NULL;

    int t = ceil((double)size / ALG_VECTOR_BASE_SIZE);
    vec->vector = ALG_MALLOC(sizeof(alg_val_type) * t * ALG_VECTOR_BASE_SIZE);
    if (vec->vector == NULL) {
        ALG_FREE(vec);
        return NULL;
    }

    vec->caps = t * ALG_VECTOR_BASE_SIZE;
    vec->size = size;

    for (int i = 0; i < size; i++)
        vec->vector[i] = init_val;

    return vec;
}

alg_val_type *alg_vector_get_val(alg_vector *vec, int pos) {
#if USE_ASSERT
    assert(vec != NULL);
#else
    if (vec == NULL)
        return NULL;
#endif
    if (pos >= vec->size)
        return NULL;
    return &vec->vector[pos];
}

alg_state alg_vector_set_val(alg_vector *vec, int pos, alg_val_type val) {
#if USE_ASSERT
    assert(vec != NULL);
#else
    if (vec == NULL)
        return ALG_ERROR;
#endif
    if (pos >= vec->size)
        return ALG_ERROR;

    vec->vector[pos] = val;
    return ALG_OK;
}

alg_state alg_vector_insert(alg_vector *vec, int pos, alg_val_type val) {
#if USE_ASSERT
    assert(vec != NULL);
#else
    if (vec == NULL)
        return ALG_ERROR;
#endif
    if (pos > vec->size)
        return ALG_ERROR;

    // Check if the vector needs to be resized
    if (vec->size >= vec->caps) {
        int new_caps = vec->caps + ALG_VECTOR_BASE_SIZE;
        alg_val_type *new_vector = ALG_REALLOC(vec->vector, new_caps * sizeof(alg_val_type));
        if (new_vector == NULL)
            return ALG_ERROR; // Memory allocation failed
        vec->vector = new_vector;
        vec->caps = new_caps;
    }

    // Move elements to the right to make space for the new value
    memmove(&vec->vector[pos + 1], &vec->vector[pos], (vec->size - pos) * sizeof(alg_val_type));
    // Insert the new value
    vec->vector[pos] = val;
    vec->size++;

    return ALG_OK;
}

alg_state alg_vector_free(alg_vector *vec) {
    if (vec == NULL)
        return ALG_ERROR;
    ALG_FREE(vec->vector);
    ALG_FREE(vec);

    return ALG_OK;
}

alg_vector *alg_vector_from_matrix_row(const alg_matrix *matrix, int row) {
    alg_vector *vec = alg_vector_create(matrix->col, GLOBAL_INIT_VAL);
    if (vec == NULL)
        return NULL;
    for (int i = 0; i < matrix->col; i++)
        alg_vector_set_val(vec, i, *alg_matrix_get_pos_val(matrix, row, i));
    return vec;
}

alg_vector *alg_vector_from_matrix_col(const alg_matrix *matrix, int col) {
    alg_vector *vec = alg_vector_create(matrix->row, GLOBAL_INIT_VAL);
    if (vec == NULL)
        return NULL;
    for (int i = 0; i < matrix->row; i++)
        alg_vector_set_val(vec, i, *alg_matrix_get_pos_val(matrix, i, col));
    return vec;
}

char *alg_vector_print_str(const alg_vector *vector) {
    if (vector == NULL)
        return NULL;
    int buf_size = vector->size * 25 + 10;
    char *str = (char *)ALG_MALLOC(buf_size);
    int pos = 0;
    pos += snprintf(str + pos, buf_size - pos, "[");
    for (int i = 0; i < vector->size; i++) {
        pos += snprintf(str + pos, buf_size - pos, "%.2f", vector->vector[i]);
        if (i != vector->size - 1)
            pos += snprintf(str + pos, buf_size - pos, " ");
    }
    pos += snprintf(str + pos, buf_size - pos, "]\n");
    return str;
}