%{
#include <stdio.h>
%}

%token NUMBER
%token ADD SUB MUL DIV ABS
%token OP CP
%token EOL

%%

calclist:
    | calclist exp EOL { printf("= %1$d 0x%1$x\n", $2); }
    | EOL
    ;

exp: factor
    | exp ADD factor { $$ = $1 + $3; }
    | exp SUB factor { $$ = $1 - $3; }
    ;

factor: term
    | factor MUL term { $$ = $1 * $3; }
    | factor DIV term { $$ = $1 / $3; }
    ;

term: NUMBER
    | ABS term { $$ = $2 >= 0 ? $2 : -$2; }
    | OP exp CP { $$ = $2; }
    ;

%%

int main(void) {
    yyparse();
}

int yyerror(char *s) {
    fprintf(stderr, "%s: %s\n", __func__, s);
}
