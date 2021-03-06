#include <stdbool.h>
#include <strings.h> // strcasecmp

bool check_name(char const ** in) {
    return (!strcasecmp(in[0], "Iggy")  && !strcasecmp(in[1], "Pop"))
        || (!strcasecmp(in[0], "James") && !strcasecmp(in[1], "Osterberg"));
}

int main(int argc, char ** argv) {
    if(argc < 2)
        return 0;
    return check_name(&argv[1]);
}
