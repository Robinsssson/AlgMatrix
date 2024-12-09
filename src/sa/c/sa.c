#include "sa.h"

sa_handle *sa_init(int dim, double temperature, double cooling_rate, double llim, double rlim, optimization function) {
    sa_handle *handle = ALG_MALLOC(sizeof(sa_handle));
    if (handle == NULL) {
        ERROR("SA HANDLE INIT ERROR");
        return NULL;
    }
    handle->current_solution = alg_vector_create(dim, 0.0);
    for (int i = 0; i < dim; i++) {
        handle->current_solution->vector[i] = alg_random_float64(llim, rlim);
    }
    handle->best_solution = alg_vector_create_like(handle->current_solution);

    handle->new_solution = alg_vector_create_like(handle->current_solution);

    handle->dim = dim;
    handle->temperature = temperature;
    handle->cooling_rate = cooling_rate;
    handle->llim = llim;
    handle->rlim = rlim;
    handle->function = function;
    handle->current_energy = function(handle->current_solution);
    handle->best_energy = handle->current_energy;
    return handle;
}

alg_state sa_fresh(sa_handle *handle) {
    for (int i = 0; i < handle->dim; i++) {
        handle->new_solution->vector[i] = handle->current_solution->vector[i] + alg_random_float64(-1, 1);
        handle->new_solution->vector[i] = MATH_CLAIM(handle->new_solution->vector[i], handle->llim, handle->rlim);
    }

    double new_energy = handle->function(handle->new_solution);

    if (new_energy < handle->current_energy
        || exp((handle->current_energy - new_energy) / handle->temperature) > alg_random_float64(0, 1)) {
        handle->current_energy = new_energy;
        for (int i = 0; i < handle->dim; i++)
            handle->current_solution->vector[i] = handle->new_solution->vector[i];
    }

    if (handle->current_energy < handle->best_energy) {
        handle->best_energy = handle->current_energy;
        for (int i = 0; i < handle->dim; i++)
            handle->best_solution->vector[i] = handle->current_solution->vector[i];
    }

    handle->temperature *= handle->cooling_rate;
    return ALG_OK;
}

alg_state sa_free(sa_handle *handle) {
    alg_vector_free(handle->best_solution);
    alg_vector_free(handle->current_solution);
    alg_vector_free(handle->new_solution);
    ALG_FREE(handle);
    return ALG_OK;
}
