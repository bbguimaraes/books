#include <ctype.h>
#include <stdio.h>

#include "calc.tab.h"

FILE *yyin;
static int seen_eof = 0;

int yylex(void) {
    if(!yyin)
        yyin = stdin;
    if(seen_eof)
        return 0;
    for(;;) {
        int c = getc(yyin);
        if(isdigit(c)) {
            int i = c - '0';
            while(isdigit(c = getc(yyin)))
                i = (10 * i) + c - '0';
            yylval = i;
            if(c == EOF)
                seen_eof = 1;
            else
                ungetc(c, yyin);
            return NUMBER;
        }
        switch(c) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '|': return ABS;
        case '(': return OP;
        case ')': return CP;
        case '\n': return EOL;
        case ' ':
        case '\t': break;
        case EOF: return 0;
        case '/':
            if((c = getc(yyin)) == '/') {
                while((c = getc(yyin)) != '\n')
                    if(c == EOF)
                        return 0;
                break;
            }
            if(c == EOF)
                seen_eof = 1;
            else
                ungetc(c, yyin);
            return DIV;
        default: {
            char s[] = "Mystery character x\n";
            sprintf(s + sizeof("Mystery character"), "%c", c);
            yyerror(s);
            break;
        }
        }
    }
}
