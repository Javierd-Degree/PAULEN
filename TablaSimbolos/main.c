#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablaSimbolos.h"
#define MAX_LINE 256

int main(int argc, char* argv[]){
  FILE *fn, *fout;
  ssize_t read;
  size_t len = MAX_LINE;
  char *line;
  char *identificador, *valor;
  tablaSimbolos* mTablaSimbolos;
  int valorNum, i, result, tmp ,isAmbitoLocal=0;

  if (argc < 3){
    printf("No hay suficientes argumentos");
    return -1;
  }

  fn = fopen(argv[1], "r");
  if (fn == NULL) {
    printf("El archivo de entrada no es válido\n");
    return -1;
  }

  fout = fopen(argv[2], "w");
  if (fout == NULL) {
    printf("El archivo de salida no es válido\n");
    fclose(fn);
    return -1;
  }

  mTablaSimbolos = crearTablaSimbolos();
  while((read = getline(&line, &len, fn)) != -1){
    identificador = line;
    valor = NULL;
    /* Buscamos el valor (Si lo hay)*/
    for(i = 0; i < len; i++){
      if(line[i] == '\t'){
        line[i] = '\0';
        valor = line+1;
      }

      if(line[i] == '\n'){
        line[i] = '\0';
      }
    }

    if(valor != NULL)
      valorNum = atoi(valor);


    if (valor == NULL){
      /* Buscamos el identificador en la tabla */
      if(isAmbitoLocal){
        tmp = buscarAmbitoLocal(mTablaSimbolos, identificador, &result);
      }else{
        tmp = buscarAmbitoGlobal(mTablaSimbolos, identificador, &result);
      }

      /* Escribimos en el fichero de salida el resultado de la busqueda */
      if (tmp){
        fprintf(fout, "%s\t%d\n", identificador, result);
      }else{
        fprintf(fout, "%s\t-1\n", identificador);
      }
    }else if(strcmp(identificador, "cierre") == 0 && valorNum == -999){
      /* Cerramos el ambito local */
      isAmbitoLocal = 0;
      limpiarAmbitoLocal(mTablaSimbolos);
      fprintf(fout, "cierre");

    }else if(valorNum >= 0){
      /* Tenemos que guardar una variable en la has table */
      if(isAmbitoLocal){
        tmp = insertarAmbitoLocal(mTablaSimbolos, identificador, valorNum);
      }else{
        tmp = insertarAmbitoGlobal(mTablaSimbolos, identificador, valorNum);
      }

      /* Escribimos en el fichero de salida el resultado de la insercion */
      if (tmp){
        fprintf(fout, "%s\n", identificador);
      }else{
        fprintf(fout, "-1\t%s\n", identificador);
      }
    }else if(valorNum < -1){
      /* Comienza un nuevo ambito */
      if(isAmbitoLocal){
        fprintf(fout, "ERROR: No se permiten funciones anidadas\n\t%s\t%s", line, valor);
      }

      tmp = insertarAmbitoGlobal(mTablaSimbolos, identificador, valorNum);
      if (tmp){
        isAmbitoLocal = 1;
        printf("%s\n", identificador);
        /* Insertamos la función en el ámbito local tambien  */
        tmp = insertarAmbitoLocal(mTablaSimbolos, identificador, valorNum);
      }else{
        fprintf(fout, "-1\t%s\n", identificador);
      }
    }else{
      fprintf(fout, "ERROR: Linea no reconocida\n\t%s\t%s", line, valor);
    }

  }

  destruirTablaSimbolos(mTablaSimbolos);

  fclose(fn);
  fclose(fout);

  return 1;
}
