%option noyywrap
%x IFILE

%{
struct bufstack {
    struct bufstack *prev;
    YY_BUFFER_STATE bs;
    int lineno;
    char *filename;
    FILE *f;
} *curbs = NULL;

char *curfilename;

#include "include.h"
%}

%%

^"#"[ \t]*include[ \t]*[\"<] { BEGIN IFILE; }

<IFILE>[^ \t\n\">]+ {
    for(int c; ((c = input()) && c != '\n'););
    ++yylineno;
    if(!newfile(yytext))
        yyterminate();
    BEGIN INITIAL;
}

<IFILE>.|\n {
    fprintf(stderr, "%s:%d bad include line\n", curfilename, yylineno);
    yyterminate();
}

<<EOF>> { if(!popfile()) yyterminate(); }
^.      { fprintf(yyout, "%4d %s", yylineno, yytext); }
^\n     { fprintf(yyout, "%4d %s", yylineno++, yytext); }
\n      { ECHO; ++yylineno; }
.       { ECHO; }

%%

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "need filename\n");
        return 1;
    }
    while(*++argv)
        if(newfile(*argv))
            yylex();
}
