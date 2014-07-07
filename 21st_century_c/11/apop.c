#include "apop.h"

void apop_vector_print(gsl_vector *data) {
    if(!data)
        printf("NULL\n");
    else {
        for(size_t i = 0; i < data->size; i++) {
            if(data->data[i] == (int) data->data[i])
                printf("% 5i", (int) data->data[i]);
            else
                printf("% 5f", data->data[i]);
            if(i < data->size - 1)
                printf(", ");
        }
        printf("\n");
    }
}

gsl_vector *apop_vector_copy(const gsl_vector *in) {
    if(!in)
        return NULL;
    gsl_vector *out = gsl_vector_alloc(in->size);
    if(!out)
        return NULL;
    gsl_vector_memcpy(out, in);
    return out;
}

double apop_vector_distance(
        const gsl_vector *ina,
        const gsl_vector *inb,
        const char metric,
        const double norm) {
    double dist = 0;
    for (size_t i=0; i< ina->size; i++)
        dist += pow(
            fabs(gsl_vector_get(ina, i) - gsl_vector_get(inb, i)),
            norm);
    return pow(dist, 1./norm);
}

gsl_rng *apop_rng_alloc(int seed) {
    static int first_use = 1;
    if (first_use) {
       first_use = 0;
       gsl_rng_env_setup();
    }
    gsl_rng *setme = gsl_rng_alloc(gsl_rng_taus2);
    gsl_rng_set(setme, seed);
    return setme;
}
