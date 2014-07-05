#include <glib.h>
#include <math.h> // NAN, isnan
#include "stopif.h"

int _test_null(void *);
float _test_isnan(float);

void test_null() {
    g_assert(_test_null(NULL) == -1);
}

int _test_null(void * inval) {
    Stopif(!inval, return -1, "inval must not be NULL");
    return 0;
}

void test_isnan() {
    g_assert(isnan(_test_isnan(NAN)));
}

float _test_isnan(float calced_val) {
    Stopif(
        isnan(calced_val), goto nanval,
        "Calced_val was NaN. Cleaning up, leaving");
    return 0;
nanval:
    return NAN;
}

void test_error_log() {
    const char * error_msg =
        "this is the error message that should be displayed\n";
    if(g_test_subprocess()) {
        error_mode = 's';
        error_log = NULL;
        Stopif(1, ;, error_msg);
    }
    g_test_trap_subprocess(NULL, 0, 0);
    g_test_trap_assert_failed();
    g_test_trap_assert_stderr(error_msg);
}

int main(int argc, char ** argv) {
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/set1/null test", test_null);
    g_test_add_func("/set1/isnan test", test_isnan);
    g_test_add_func("/set1/error_log test", test_error_log);
    error_log = fopen("/dev/null", "w");
    return g_test_run();
}
