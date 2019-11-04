%{
#include <stdio.h>
#include "tokens.h"

long nline=0, ncolumn=0;
%}

%option noyywrap

puntoycoma ";"
coma ","
parentesisizquierdo "("
parentesisderecho ")"
corcheteizquierdo "["
corchetederecho "]"
llaveizquierda "{"
llavederecha "}"
asignacion "="
mas "+"
menos "-"
division "/"
asterisco "*"
and "&&"
or "||"
not "!"
igual "=="
distinto "!="
menorigual "<="
mayorigual ">="
menor "<"
mayor ">"


%%
main { return TOK_MAIN; }
int { return TOK_INT; }
boolean { return TOK_BOOLEAN; }
array { return TOK_ARRAY; }
function { return TOK_FUNCTION; }
if { return TOK_IF; }
else { return TOK_ELSE; }
while { return TOK_WHILE; }
scanf { return TOK_SCANF; }
printf { return TOK_PRINTF; }
return { return TOK_RETURN; }


puntoycoma { return TOK_PUNTOYCOMA; }
coma { return TOK_COMA; }
parentesisizquierdo { return TOK_PARENTESISIZQUIERDO; }
parentesisderecho { return TOK_PARENTESISDERECHO; }
corcheteizquierdo { return TOK_CORCHETEIZQUIERDO; }
corchetederecho { return TOK_CORCHETEDERECHO; }
llaveizquierda { return TOK_LLAVEIZQUIERDA; }
llavederecha { return TOK_LLAVEDERECHA; }
asignacion { return TOK_ASIGNACION; }
mas { return TOK_MAS; }
menos { return TOK_MENOS; }
division { return TOK_DIVISION; }
asterisco { return TOK_ASTERISCO; }
and { return TOK_AND; }
or { return TOK_OR; }
not { return TOK_NOT; }
igual { return TOK_IGUAL; }
distinto { return TOK_DISTINTO; }
menorigual { return TOK_MENORIGUAL; }
mayorigual { return TOK_MAYORIGUAL; }
menor { return TOK_MENOR; }
mayor { return TOK_MAYOR; }

true { return TOK_TRUE; }
false { return TOK_FALSE; }

\n { nline++; ncolumn=0; }

[a-zA-Z][0-9a-zA-Z]{0,99} { return TOK_IDENTIFICADOR; }

[0-9]+ { return TOK_CONSTANTE_ENTERA; }

[^ \t\n(//.*\n)]+ { return TOK_ERROR; }

%%
