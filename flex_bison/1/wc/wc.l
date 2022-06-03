%{
static int lines = 0;
static int words = 0;
static int chars = 0;
%}

%%

[^ \t\n\r\f\v]+ { ++words; chars += strlen(yytext); }
\n              { ++lines; ++chars; }
.               { ++chars; }

%%

int main(void) {
    yylex();
    printf("%d %d %d\n", lines, words, chars);
}
