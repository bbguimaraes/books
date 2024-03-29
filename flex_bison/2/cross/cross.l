%option noyywrap nodefault yylineno

%x COMMENT
%x IFILE

UCN  (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
EXP  ([Ee][-+]?[0-9]+)
ILEN ([Uu](L|l|LL|ll)?|(L|l|LL|ll)[Uu]?)

%{
struct bufstack {
    struct bufstack *prev;
    YY_BUFFER_STATE bs;
    int lineno;
    char *filename;
    FILE *f;
} *curbs = NULL;

char *curfilename;

#include "../concordance/ref.h"
#include "../include/include.h"

int defining;
%}

%%

"/*"          { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>([^*]|\n)+|.

<COMMENT><<EOF>> {
    printf("%s:%d: Unterminated comment\n", curfilename, yylineno);
    return 0;
}

"//".*\n

_Bool |
_Complex |
_Imaginary |
auto |
char |
const |
double |
enum |
extern |
float |
inline |
int |
long |
register |
restrict |
short |
signed |
static |
struct |
typedef |
union |
unsigned |
void |
volatile { defining = 1; }

break
case
continue
default
do
else
for
goto
if
return
sizeof
switch
while

0[0-7]*{ILEN}?
[1-9][0-9]*{ILEN}?
0[Xx][0-9a-fA-F]+{ILEN}?
([0-9]*\.[0-9]+|[0-9]+\.){EXP}?[flFL]?
[0-9]+{EXP}[flFL]?

0[Xx]([0-9a-fA-F]*\.[0-9a-fA-F]+|[0-9a-fA-F]+\.?)[Pp][-+]?[0-9]+[flFL]?

\'([^'\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9a-fA-F]+|{UCN})+\'
L?\"([^"\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9a-fA-F]+|{UCN})*\"

"{"|"<%"|";" { defining = 0; }

"["|"]"|"("|")"|"{"|"}"|"."|"->"
"++"|"--"|"&"|"*"|"+"|"-"|"~"|"!"
"/"|"%"|"<<"|">>"|"<"|">"|"<="|">="|"=="|"!="|"^"|"|"|"&&"|"||"
"?"|":"|";"|"..."
"="|"*="|"/="|"%="|"+="|"-="|"<<="|">>="|"&="|"^="|"|="
","|"#"|"##"
"<:"|":>"|"%>"|"%:"|"%:%:"

([_a-zA-Z]|{UCN})([_a-zA-Z0-9]|{UCN})* {
    addref(yylineno, curfilename, yytext, defining);
}

[ \t\n]+
\\$

"#"" "*if.*\n
"#"" "*else.*\n
"#"" "*endif.*\n
"#"" "*define.*\n
"#"" "*line.*\n

^"#"[ \t]*include[ \t]*[\"<] { BEGIN IFILE; }

<IFILE>[^>\"]+ {
    for(int c; ((c = input()) && c != '\n'););
    if(!newfile(strdup(yytext)))
        yyterminate();
    BEGIN INITIAL;
}

<IFILE>.|\n {
    fprintf(stderr, "%s:%d bad include line\n", curfilename, yylineno);
    yyterminate();
}

<<EOF>> { if(!popfile()) yyterminate(); }

. { printf("%s:%d: Mystery character '%s'\n", curfilename, yylineno, yytext); }

%%

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "need filename\n");
        return 1;
    }
    while(*++argv)
        if(newfile(*argv))
            yylex();
    printrefs();
}
