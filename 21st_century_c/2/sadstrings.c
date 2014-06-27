#include <stdio.h>
#include <string.h> // strlen
#include <stdlib.h> // malloc, free, system

void get_strings(char const * in) {
    char * cmd;
    malloc(1);
    int len = strlen("strings ") + strlen(in) + 1;
    cmd = malloc(len);
    snprintf(cmd, len, "strings %s", in);
    if(system(cmd)) fprintf(stderr, "something when wrong running %s.\n", cmd);
    free(cmd);
    free(cmd);
}

int main(int argc, char ** argv) {
    get_strings(argv[0]);
}
