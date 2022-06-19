%option noyywrap nodefault yylineno case-insensitive

%{
char *curfilename;

#include "ref.h"
%}

%%

a |
an |
and |
are |
as |
at |
be |
but |
for |
in |
is |
it |
of |
on |
or |
that |
the |
this |
to

[a-z]+(\'(s|t))? { addref(yylineno, curfilename, yytext, 0); }
.|\n

%%

int main(int argc, char **argv) {
    if(argc < 2) {
        curfilename = "(stdin)";
        yylineno = 1;
        yylex();
    } else while(*++argv) {
        FILE *f = fopen(*argv, "r");
        if(!f) {
            perror(*argv);
            return 1;
        }
        curfilename = *argv;
        yyrestart(f);
        yylineno = 1;
        yylex();
        fclose(f);
    }
    printrefs();
}
