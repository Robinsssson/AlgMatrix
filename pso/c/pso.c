#include "pso.h"
#include "../../c_lib/alg_lib.h"
#include <stdlib.h>
#include <string.h>

pso_handle *pso_init(int number, int dim, double w, double c1, double c2,
                     pso_aim_function function) {
    pso_handle *pso_structure = malloc(sizeof(pso_handle));
    if (pso_structure == NULL)
        return NULL;
    pso_structure->vec = alg_matrix_create(number, dim);
    if (pso_structure->vec == NULL) {
        free(pso_structure);
        return NULL;
    }
    pso_structure->position = alg_matrix_create(number, dim);
    if (pso_structure->position == NULL) {
        free(pso_structure->vec);
        free(pso_structure);
        return NULL;
    }
    pso_structure->p_best = alg_matrix_copy(pso_structure->position);
    if (pso_structure->p_best == NULL) {
        free(pso_structure->vec);
        free(pso_structure->position);
        free(pso_structure);
        return NULL;
    }
    pso_structure->g_best = alg_matrix_create(number, dim);
    if (pso_structure->p_best == NULL) {
        free(pso_structure->p_best);
        free(pso_structure->vec);
        free(pso_structure->position);
        free(pso_structure);
        return NULL;
    }
    pso_structure->func = function;
    pso_structure->c1 = c1;
    pso_structure->c2 = c2;
    pso_structure->dim = dim;
    pso_structure->number = number;
    pso_structure->w = w;

    return pso_structure;
}
void pso_fresh(pso_handle *pso_structure);
pso_handle *pso_free(pso_handle *pso_structure) {
    if (pso_structure == NULL)
        return NULL;
    if (pso_structure->g_best)
        free(pso_structure->g_best);
    if (pso_structure->p_best)
        free(pso_structure->p_best);
    if (pso_structure->position)
        free(pso_structure->position);
    if (pso_structure->vec)
        free(pso_structure->vec);
    if (pso_structure)
        free(pso_structure);
    return NULL;
}