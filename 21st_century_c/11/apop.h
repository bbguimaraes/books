#include <math.h>
#include <gsl/gsl_nan.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sys.h>
#include <gsl/gsl_vector.h>

void apop_vector_print(gsl_vector *data);
gsl_vector *apop_vector_copy(const gsl_vector *in);
double apop_vector_distance(
    const gsl_vector *ina, const gsl_vector *inb,
    const char metric, const double norm);
gsl_rng *apop_rng_alloc(int seed);
