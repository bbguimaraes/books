%{
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"
%}

%union {
    struct ast *a;
    double d;
}

%token <d> NUMBER
%token EOL

%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

%type <a> exp

%%

calclist:
    | calclist exp EOL {
        printf("= %4.4g\n", eval($2));
        treefree($2);
        printf("> ");
    }
    | calclist EOL { printf("> "); }
    ;

exp:  exp '+' exp { $$ = newast('+', $1, $3); }
    | exp '-' exp { $$ = newast('-', $1, $3); }
    | exp '*' exp { $$ = newast('*', $1, $3); }
    | exp '/' exp { $$ = newast('/', $1, $3); }
    | '|' exp     { $$ = newast('|', $2, NULL); }
    | NUMBER      { $$ = newnum($1); }
    | '-' exp %prec UMINUS { $$ = newast('M', $2, NULL); }
    | '(' exp ')' { $$ = $2; }
    ;

%%
