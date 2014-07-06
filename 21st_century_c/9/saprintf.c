#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h> // free

// Safer asprintf macro
#define Sasprintf(write_to, ...) {             \
    char * tmp_string_for_extend = (write_to); \
    asprintf(&(write_to), __VA_ARGS__);        \
    free(tmp_string_for_extend);               \
}

int main() {
    int i = 3;
    char * q = NULL;
    Sasprintf(q, "select * from tab");
    Sasprintf(q, "%s where col%i is not null", q, i);
    printf("%s\n", q);
}
