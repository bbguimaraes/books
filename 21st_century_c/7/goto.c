#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include <math.h> // isnan

double sum_to_first_nan(
        double * vector, int vector_size,
        double * vector2, int vector2_size, int * error) {
    double sum = 0;
    *error = 1;
    for(int i = 0; i < vector_size; ++i) {
        if(isnan(vector[i]))
            goto outro;
        sum += vector[i];
    }
    for(int i = 0; i < vector2_size; ++i) {
        if(isnan(vector2[i]))
            goto outro;
        sum += vector2[i];
    }
    *error = 0;
outro:
    printf("The sum until the first Nan (if any) was %g\n", sum);
    free(vector);
    free(vector2);
    return sum;
}

int main() {
    double * v1 = malloc(5 * sizeof(double));
    double * v2 = malloc(5 * sizeof(double));
    double tmp[] = { 1, 2, 3, 4, 5 };
    memcpy(v1, tmp, 5 * sizeof(double));
    memcpy(v2, tmp, 5 * sizeof(double));
    int error;
    sum_to_first_nan(v1, 5, v2, 5, &error);
    printf("%i\n", error);
    v2[2] = NAN;
    sum_to_first_nan(v1, 5, v2, 5, &error);
    printf("%i\n", error);
}
