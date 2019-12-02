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
  int valorNum, i, result, tmp ,isAmbitoLocal=0, localIndex = 0;

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
  line = (char*)malloc(sizeof(char)*MAX_LINE);
  while((read = getline(&line, &len, fn)) != -1){
    identificador = line;
    valor = NULL;
    /* Buscamos el valor (Si lo hay)*/
    for(i = 0; i < len; i++){
      if(line[i] == '\t' || line[i] == ' '){
        line[i] = '\0';
        valor = &line[i+1];
      }

      if(line[i] == '\n' || line[i] == '\r'){
        line[i] = '\0';
        break;
      }
    }

    if(valor != NULL)
      valorNum = atoi(valor);


    if (valor == NULL){
      /* Buscamos el identificador en la tabla */
      if(isAmbitoLocal){
        tmp = buscarAmbitoLocal(mTablaSimbolos, identificador, localIndex, &result);
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
      limpiarAmbitoLocal(mTablaSimbolos, localIndex);
      --localIndex;
      if (localIndex < 0){
        isAmbitoLocal = 0;
        localIndex = 0;
      }
      fprintf(fout, "cierre\n");

    }else if(valorNum >= 0){
      /* Tenemos que guardar una variable en la has table */
      if(isAmbitoLocal){
        tmp = insertarAmbitoLocal(mTablaSimbolos, identificador, localIndex, valorNum);
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
      if (isAmbitoLocal == 0){
        tmp = insertarAmbitoGlobal(mTablaSimbolos, identificador, valorNum);
        if (tmp){
          fprintf(fout, "%s\n", identificador);
          isAmbitoLocal = 1;
          /* Insertamos la función en el ámbito local tambien  */
          tmp = insertarAmbitoLocal(mTablaSimbolos, identificador, localIndex, valorNum);
        }else{
          fprintf(fout, "-1\t%s\n", identificador);
        }
      }else{
        tmp = insertarAmbitoLocal(mTablaSimbolos, identificador, localIndex, valorNum);
        if (tmp){
          fprintf(fout, "%s\n", identificador);
          localIndex++;
          /* Insertamos la función en el ámbito local nuevo tambien  */
          tmp = insertarAmbitoLocal(mTablaSimbolos, identificador, localIndex, valorNum);
        }else{
          fprintf(fout, "-1\t%s\n", identificador);
        }
      }
    }else{
      fprintf(fout, "ERROR: Linea no reconocida\n\t%s\t%s", line, valor);
    }

  }

  borrarTablaSimbolos(mTablaSimbolos);
  free(line);
  fclose(fn);
  fclose(fout);

  return 1;
}
