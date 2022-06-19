%x LINE

%%

^.  { printf("line\n  %s", yytext); BEGIN LINE; }
^\n { puts("line"); }

<LINE>\n { putchar('\n'); BEGIN INITIAL; }
<LINE>.+ { printf("%s", yytext); }
