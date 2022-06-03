%{
#include "calc.tab.h"
%}

%%

"+"      { return ADD; }
"-"      { return SUB; }
"*"      { return MUL; }
"/"      { return DIV; }
"|"      { return ABS; }
"&"      { return AND; }
"("      { return OP; }
")"      { return CP; }
[0-9]+   { yylval = atoi(yytext); return NUMBER; }
0x[0-9]+ { yylval = strtol(yytext, NULL, 16); return NUMBER; }
"//".*
\n       { return EOL; }
[ \t]    {}
.        { printf("Mystery character %c\n", *yytext); }

%%
