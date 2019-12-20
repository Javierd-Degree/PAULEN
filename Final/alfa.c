#include <stdio.h>
#include "alfa.h"
#include "tablaSimbolos.h"

#if YYDEBUG == 1
   extern yydebug;
   yydebug = 1;
#endif

int yylex();
int yyparse();

int main(int argc, char *argv[]){
  extern FILE *yyin, *out;
  extern tablaSimbolos *tabla;
  int result;

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

  tabla = crearTablaSimbolos();

  result = yyparse();
  if (result == -1){
    printf("Compilación fallida.\n");
  }


  fclose(yyin);
  fclose(out);
  borrarTablaSimbolos(tabla);

  return 0;
}
