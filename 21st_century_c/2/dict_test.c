/**
 * \file dict_test.c
 * \mainpage Dictionary testing module.
 * This module is used to test the dictionary library created on chapter 11.
 */
#include <glib.h>
#include "dict.h"

/**
 * Fixture used in most tests.
 */
typedef struct {
    dictionary * dd;
} dfixture;

/**
 * Sets up a dictionary for testing.
 */
void dict_setup(dfixture * df, gconstpointer test_data) {
    df->dd = dictionary_new();
    dictionary_add(df->dd, "key1", "val1");
    dictionary_add(df->dd, "key2", NULL);
}

/**
 * Releases the test dictionary.
 */
void dict_teardown(dfixture * df, gconstpointer test_data) {
    dictionary_free(df->dd);
}

/**
 * Checks getting values from keys.
 */
void check_keys(dictionary const * d) {
    char * got_it = dictionary_find(d, "xx");
    g_assert(got_it == dictionary_not_found);
    got_it = dictionary_find(d, "key1");
    g_assert_cmpstr(got_it, ==, "val1");
    got_it = dictionary_find(d, "key2");
    g_assert_cmpstr(got_it, ==, NULL);
}

/**
 * Tests the creation of a dictionary.
 */
void test_new(dfixture * df, gconstpointer ignore) {
    check_keys(df->dd);
}

/**
 * Tests copying a dictionary.
 */
void test_copy(dfixture * df, gconstpointer ignored) {
    dictionary * cp = dictionary_copy(df->dd);
    check_keys(cp);
    dictionary_free(cp);
}

/**
 * Tests that passing a NULL key aborts the program with a useful error
 * message.
 */
void test_failure() {
    if(g_test_subprocess()) {
        dictionary * dd = dictionary_new();
        dictionary_add(dd, NULL, "blank");
    }
    g_test_trap_subprocess(NULL, 0, 0);
    g_test_trap_assert_failed();
    g_test_trap_assert_stderr("NULL is not a valid key.\n");
}

/**
 * Entry point.
 */
int main(int argc, char ** argv) {
    g_test_init(&argc, &argv, NULL);
    g_test_add(
        "/set1/new test",
        dfixture, NULL, dict_setup, test_new, dict_teardown);
    g_test_add(
        "/set1/copy test",
        dfixture, NULL, dict_setup, test_copy, dict_teardown);
    g_test_add_func("/set2/fail test", test_failure);
    return g_test_run();
}
