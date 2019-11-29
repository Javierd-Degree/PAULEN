%{
#include <stdio.h>
#include <string.h>

#include "alfa.tab.h"

long nline=1, ncolumn=1;
/* Variable usada para indicar cuando un error es morfologico y por tanto, se imprime aqui, no en yyerror */
int yymorph = 0;
extern YYSTYPE yylval;
extern FILE *out;
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
\n { yymorph=0; nline++; ncolumn=1; }
" " { yymorph=0; ncolumn+=yyleng; }
\t { yymorph=0; ncolumn+=yyleng; }
"//".* { yymorph=0; }

main { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_MAIN; }
int { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_INT; }
boolean { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_BOOLEAN; }
array { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_ARRAY; }
function { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_FUNCTION; }
if { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_IF; }
else { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_ELSE; }
while { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_WHILE; }
scanf { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_SCANF; }
printf { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_PRINTF; }
return { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_RETURN; }


{puntoycoma} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_PUNTOYCOMA; }
{coma} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_COMA; }
{parentesisizquierdo} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_PARENTESISIZQUIERDO; }
{parentesisderecho} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_PARENTESISDERECHO; }
{corcheteizquierdo} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_CORCHETEIZQUIERDO; }
{corchetederecho} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_CORCHETEDERECHO; }
{llaveizquierda} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_LLAVEIZQUIERDA; }
{llavederecha} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_LLAVEDERECHA; }
{asignacion} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_ASIGNACION; }
{mas} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_MAS; }
{menos} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_MENOS; }
{division} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_DIVISION; }
{asterisco} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_ASTERISCO; }
{and} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_AND; }
{or} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_OR; }
{not} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_NOT; }
{igual} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_IGUAL; }
{distinto} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_DISTINTO; }
{menorigual} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_MENORIGUAL; }
{mayorigual} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_MAYORIGUAL; }
{menor} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_MENOR; }
{mayor} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_MAYOR; }

true { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_TRUE; }
false { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); return TOK_FALSE; }

[a-zA-Z][0-9a-zA-Z]{0,99} { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); yylval.identificador = strdup(yytext); return TOK_IDENTIFICADOR; }
[a-zA-Z][0-9a-zA-Z]{100,} { yymorph=1; fprintf(stderr, "****Error en [lin %li, col %li]: idenficador demasiado largo(%s)\n", nline, ncolumn ,yytext); return TOK_ERROR; }

[0-9]+ { yymorph=0; ncolumn+=yyleng; fprintf(out, ";D:\t%s\n", yytext); yylval.numero = atoi(yytext); return TOK_CONSTANTE_ENTERA; }


. { yymorph=1; fprintf(stderr, "****Error en [lin %li, col %li]: símbolo no permitido (%s)\n", nline, ncolumn, yytext); return TOK_ERROR; }

%%
