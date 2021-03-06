#define _GNU_SOURCE // get stdio.h to define asprintf
#include "string_utilities.h" // string_from_file
#include "process_dir.h"
#include "unictr.h"
#include <glib.h>
#include <stdlib.h> // free

void hash_a_file(filestruct path) {
    GHashTable * hash = path.data;
    char * sf = string_from_file(path.fullname);
    if(!sf) return;
    char * sf_copy = sf;
    if(g_utf8_validate(sf, -1, NULL)) {
        gunichar uc;
        while((uc = g_utf8_get_char(sf)) != '\0') {
            hash_a_character(uc, hash);
            sf = g_utf8_next_char(sf);
        }
    }
    free(sf_copy);
}

int main(int argc, char ** argv) {
    GHashTable * hash;
    hash = new_unicode_counting_hash();
    char * start = NULL;
    if(argc > 1) asprintf(&start, "%s", argv[1]);
    printf("Hashing %s\n", start ? start : "the current directory");
    process_dir(.name=start, .file_action=hash_a_file, .data=hash);
    g_hash_table_foreach(hash, printone, NULL);
}
