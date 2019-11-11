%{
#include <stdio.h>
#include "tokens.h"

long nline=1, ncolumn=1;
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
\n { nline++; ncolumn=1; }
" " { ncolumn+=yyleng; }
\t { ncolumn+=yyleng; }
"//".* { }

main { ncolumn+=yyleng; return TOK_MAIN; }
int { ncolumn+=yyleng; return TOK_INT; }
boolean { ncolumn+=yyleng; return TOK_BOOLEAN; }
array { ncolumn+=yyleng; return TOK_ARRAY; }
function { ncolumn+=yyleng; return TOK_FUNCTION; }
if { ncolumn+=yyleng; return TOK_IF; }
else { ncolumn+=yyleng; return TOK_ELSE; }
while { ncolumn+=yyleng; return TOK_WHILE; }
scanf { ncolumn+=yyleng; return TOK_SCANF; }
printf { ncolumn+=yyleng; return TOK_PRINTF; }
return { ncolumn+=yyleng; return TOK_RETURN; }


{puntoycoma} { ncolumn+=yyleng; return TOK_PUNTOYCOMA; }
{coma} { ncolumn+=yyleng; return TOK_COMA; }
{parentesisizquierdo} { ncolumn+=yyleng; return TOK_PARENTESISIZQUIERDO; }
{parentesisderecho} { ncolumn+=yyleng; return TOK_PARENTESISDERECHO; }
{corcheteizquierdo} { ncolumn+=yyleng; return TOK_CORCHETEIZQUIERDO; }
{corchetederecho} { ncolumn+=yyleng; return TOK_CORCHETEDERECHO; }
{llaveizquierda} { ncolumn+=yyleng; return TOK_LLAVEIZQUIERDA; }
{llavederecha} { ncolumn+=yyleng; return TOK_LLAVEDERECHA; }
{asignacion} { ncolumn+=yyleng; return TOK_ASIGNACION; }
{mas} { ncolumn+=yyleng; return TOK_MAS; }
{menos} { ncolumn+=yyleng; return TOK_MENOS; }
{division} { ncolumn+=yyleng; return TOK_DIVISION; }
{asterisco} { ncolumn+=yyleng; return TOK_ASTERISCO; }
{and} { ncolumn+=yyleng; return TOK_AND; }
{or} { ncolumn+=yyleng; return TOK_OR; }
{not} { ncolumn+=yyleng; return TOK_NOT; }
{igual} { ncolumn+=yyleng; return TOK_IGUAL; }
{distinto} { ncolumn+=yyleng; return TOK_DISTINTO; }
{menorigual} { ncolumn+=yyleng; return TOK_MENORIGUAL; }
{mayorigual} { ncolumn+=yyleng; return TOK_MAYORIGUAL; }
{menor} { ncolumn+=yyleng; return TOK_MENOR; }
{mayor} { ncolumn+=yyleng; return TOK_MAYOR; }

true { ncolumn+=yyleng; return TOK_TRUE; }
false { ncolumn+=yyleng; return TOK_FALSE; }

[a-zA-Z][0-9a-zA-Z]{0,99} { ncolumn+=yyleng; return TOK_IDENTIFICADOR; }
[a-zA-Z][0-9a-zA-Z]{100,} { return TOK_ERROR; }

[0-9]+ { ncolumn+=yyleng; return TOK_CONSTANTE_ENTERA; }



. { return TOK_ERROR; }

%%
