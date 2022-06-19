struct symbol {
    char *name;
    struct ref *reflist;
};

struct ref {
    struct ref *next;
    char *filename;
    int flags;
    int lineno;
};

#define NHASH 9997
struct symbol symtab[NHASH];

static struct symbol *lookup(char *sym);
static void addref(int lineno, char *filename, char *word, int flags);
static void printrefs(void);

static unsigned symhash(char *sym) {
    unsigned int hash = 0;
    for(unsigned c; c = *sym++;)
        hash = hash * 9 ^ c;
    return hash;
}

static struct symbol *lookup(char *sym) {
    struct symbol *sp = &symtab[symhash(sym) % NHASH];
    int scount = NHASH;
    while(--scount >= 0) {
        if(sp->name && strcmp(sp->name, sym) == 0)
            return sp;
        if(!sp->name) {
            sp->name = strdup(sym);
            sp->reflist = 0;
            return sp;
        }
        if(++sp >= symtab + NHASH)
            sp = symtab;
    }
    fputs("symbol table overflow\n", stderr);
    abort();
}

static void addref(int lineno, char *filename, char *word, int flags) {
    struct symbol *sp = lookup(word);
    if(sp->reflist)
        if(sp->reflist->lineno == lineno && sp->reflist->filename == filename)
            return;
    struct ref *r = malloc(sizeof(struct ref));
    if(!r) {
        fputs("out of space\n", stderr);
        abort();
    }
    *r = (struct ref){
        .next = sp->reflist,
        .filename = filename,
        .flags = flags,
        .lineno = lineno,
    };
    sp->reflist = r;
}

static int symcompare(const void *xa, const void *xb) {
    const struct symbol *a = xa;
    const struct symbol *b = xb;
    if(!a->name) {
        if(!b->name)
            return 0;
        return 1;
    }
    if(!b->name)
        return -1;
    return strcmp(a->name, b->name);
}

static void printrefs(void) {
    qsort(symtab, NHASH, sizeof(*symtab), symcompare);
    for(struct symbol *sp = symtab; sp->name && sp < symtab + NHASH; ++sp) {
        char *prevfn = NULL;
        struct ref *rp = sp->reflist;
        struct ref *rpp = NULL;
        do {
            struct ref *rpn = rp->next;
            rp->next = rpp;
            rpp = rp;
            rp = rpn;
        } while(rp);
        printf("%10s", sp->name);
        for(rp = rpp; rp; rp = rp->next) {
            if(rp->filename == prevfn)
                printf(" %d", rp->lineno);
            else {
                printf(" %s:%d", rp->filename, rp->lineno);
                prevfn = rp->filename;
            }
            if(rp->flags & 1)
                putchar('*');
        }
        putchar('\n');
    }
}
