#include <stdio.h>
#include "tokens.h"
#include "lex.yy.c"

extern FILE* yyin;
extern char* yytext;
extern int yyleng;

char *getName(int code){
  switch (code) {
    case TOK_MAIN:
      return "TOK_MAIN";
    case TOK_INT:
      return "TOK_INT";
    case TOK_BOOLEAN:
      return "TOK_BOOLEAN";
    case TOK_ARRAY:
      return "TOK_ARRAY";
    case TOK_FUNCTION:
      return "TOK_FUNCTION";
    case TOK_IF:
      return "TOK_IF";
    case TOK_ELSE:
      return "TOK_ELSE";
    case TOK_WHILE:
      return "TOK_WHILE";
    case TOK_SCANF:
      return "TOK_SCANF";
    case TOK_PRINTF:
      return "TOK_PRINTF";
    case TOK_RETURN:
      return "TOK_RETURN";
    case TOK_PUNTOYCOMA:
      return "TOK_PUNTOYCOMA";
    case TOK_COMA:
      return "TOK_COMA";
    case TOK_PARENTESISIZQUIERDO:
      return "TOK_PARENTESISIZQUIERDO";
    case TOK_PARENTESISDERECHO:
      return "TOK_PARENTESISDERECHO";
    case TOK_CORCHETEIZQUIERDO:
      return "TOK_CORCHETEIZQUIERDO";
    case TOK_CORCHETEDERECHO:
      return "TOK_CORCHETEDERECHO";
    case TOK_LLAVEIZQUIERDA:
      return "TOK_LLAVEIZQUIERDA";
    case TOK_LLAVEDERECHA:
      return "TOK_LLAVEDERECHA";
    case TOK_ASIGNACION:
      return "TOK_ASIGNACION";
    case TOK_MAS:
      return "TOK_MAS";
    case TOK_MENOS:
      return "TOK_MENOS";
    case TOK_DIVISION:
      return "TOK_DIVISION";
    case TOK_ASTERISCO:
      return "TOK_ASTERISCO";
    case TOK_AND:
      return "TOK_AND";
    case TOK_OR:
      return "TOK_OR";
    case TOK_NOT:
      return "TOK_NOT";
    case TOK_DISTINTO:
      return "TOK_DISTINTO";
    case TOK_MENORIGUAL:
      return "TOK_MENORIGUAL";
    case TOK_MAYORIGUAL:
      return "TOK_MAYORIGUAL";
    case TOK_MENOR:
      return "TOK_MENOR";
    case TOK_MAYOR:
      return "TOK_MAYOR";
    case TOK_IDENTIFICADOR:
      return "TOK_IDENTIFICADOR";
    case TOK_CONSTANTE_ENTERA:
      return "TOK_CONSTANTE_ENTERA";
    case TOK_TRUE:
      return "TOK_TRUE";
    case TOK_FALSE:
      return "TOK_FALSE";
    case TOK_ERROR:
      return "TOK_ERROR";
  }
}

int main(int argc, char *argv[]){
  FILE *fout;
  int code;

  if (argc < 3){
    printf("No hay suficientes argumentos");
    return -1;
  }

  yyin = fopen(agv[1], "r");
  if (yyin == NULL) {
    printf("El archivo de entrada no es válido\n");
    return -1;
  }

  fout = fopen(agv[2], "r");
  if (fout == NULL) {
    printf("El archivo de salida no es válido\n");
    fclose(fin);
    return -1;
  }

  do{
    code = yylex();
    /* TODO Ver que pasa cuando es un error */
    if (code == TOK_ERROR && yyleng > 100){
      fprintf(stderr, "****Error en [lin %d, col %d]: idenficador demasiado largo\n(%s)", ,yytext);
      break;
    }else if(code == TOK_ERROR){
      fprintf(stderr, "****Error en [lin %d, col %d]: símbolo no permitido (%s)", yytext);
      break;
    }

    fprintf(fout, "%s\t%d\t%s\n", getName(code), code, yytext);

  }while(code != 0);

  fclose(yyin);
  fclose(fout);

  return 0;
}
