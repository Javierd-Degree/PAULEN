#include <stdio.h>
#if YYDEBUG == 1
   extern yydebug;
   yydebug = 1;
#endif

int yylex();
int yyparse();

int main(int argc, char *argv[]){
  extern FILE *yyin, *out;

  if (argc < 3){
    printf("No hay suficientes argumentos\n");
    return -1;
  }

  yyin = fopen(argv[1], "r");
  if (yyin == NULL) {
    printf("El archivo de entrada no es válido\n");
    return -1;
  }

  out = fopen(argv[2], "w");
  if (out == NULL) {
    printf("El archivo de salida no es válido\n");
    fclose(yyin);
    return -1;
  }

  yyparse();
  

  fclose(yyin);
  fclose(out);

  return 0;
}
