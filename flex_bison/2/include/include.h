static int newfile(char *fn) {
    FILE *f = fopen(fn, "r");
    struct bufstack *bs = malloc(sizeof(*bs));
    if(!f) {
        perror(fn);
        return 0;
    }
    if(!bs) {
        perror("malloc");
        exit(1);
    }
    if(curbs)
        curbs->lineno = yylineno;
    *bs = (struct bufstack){
        .prev = curbs,
        .bs = yy_create_buffer(f, YY_BUF_SIZE),
        .filename = fn,
        .f = f,
    };
    yy_switch_to_buffer(bs->bs);
    curbs = bs;
    yylineno = 1;
    curfilename = fn;
    return 1;
}

static int popfile(void) {
    struct bufstack *bs = curbs;
    if(!bs)
        return 0;
    fclose(bs->f);
    yy_delete_buffer(bs->bs);
    struct bufstack *prevbs = bs->prev;
    free(bs);
    if(!prevbs)
        return 0;
    yy_switch_to_buffer(prevbs->bs);
    curbs = prevbs;
    yylineno = curbs->lineno;
    curfilename = curbs->filename;
    return 1;
}
