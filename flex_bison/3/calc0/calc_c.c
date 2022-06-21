#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

struct ast *newast(int nodetype, struct ast *l, struct ast *r) {
    struct ast *ret = malloc(sizeof(*ret));
    if(!ret) {
        yyerror("out of space");
        exit(1);
    }
    *ret = (struct ast){
        .nodetype = nodetype,
        .l = l,
        .r = r,
    };
    return ret;
}

struct ast *newnum(double d) {
    struct numval *ret = malloc(sizeof(*ret));
    if(!ret) {
        yyerror("out of space");
        exit(1);
    }
    *ret = (struct numval){
        .nodetype = 'K',
        .number = d,
    };
    return (struct ast*)ret;
}

double eval(struct ast *a) {
    switch(a->nodetype) {
    case 'K': return ((struct numval*)a)->number;
    case '+': return eval(a->l) + eval(a->r);
    case '-': return eval(a->l) - eval(a->r);
    case '*': return eval(a->l) * eval(a->r);
    case '/': return eval(a->l) / eval(a->r);
    case '|': double v = eval(a->l); return v < 0 ? -v : v;
    case 'M': return -eval(a->l);
    default:
        fprintf(stderr, "internal error: bad node %c\n", a->nodetype);
        return 0;
    }
}

void treefree(struct ast *a) {
    switch(a->nodetype) {
    case '+':
    case '-':
    case '*':
    case '/': treefree(a->r); [[fallthrough]]
    case '|':
    case 'M': treefree(a->l); [[fallthrough]]
    case 'K': free(a); break;
    default: fprintf(stderr, "internal error: free bad node %c\n", a->nodetype);
    }
}

void yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fputc('\n', stderr);
}

int main(void) {
    printf("> ");
    return yyparse();
}
