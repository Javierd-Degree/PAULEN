
#include "uthash.h"
#include "tablaSimbolos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LOCALS 6

struct _entradaTabla{
  char* key;
  int value;
  UT_hash_handle hh;
};

struct _tablaSimbolos{
  entradaTabla** ambitoLocal;
  entradaTabla** ambitoGlobal;
};

/****************************************************************************/

entradaTabla* crearEntrada(int len){
  entradaTabla* entrada;
  entrada = (entradaTabla*)malloc(sizeof(entradaTabla));
  entrada->key = (char*)malloc(sizeof(char)*len);
  return entrada;
}

int addValue(entradaTabla** tabla, char* key, int index, int value){
  entradaTabla* entrada;
  HASH_FIND_STR(tabla[index], key, entrada);
  if (entrada == NULL) {
    entrada = crearEntrada(strlen(key)+1);
    strcpy(entrada->key, key);
    entrada->value = value;
    HASH_ADD_STR(tabla[index], key, entrada);
    return 1;
  }
  return 0;
}

entradaTabla* findValue(entradaTabla** tabla, int index, char* key) {
    entradaTabla* value;
    if(index < 0 || index >= MAX_LOCALS){
      return NULL;
    }

    if(tabla[index] == NULL){
      return NULL;
    }

    HASH_FIND_STR(tabla[index], key, value );  /* value: output pointer */
    return value;
}

void delete_all(entradaTabla** tabla, int index) {
  entradaTabla *current_item, *tmp;

  HASH_ITER(hh, tabla[index], current_item, tmp) {
    free(current_item->key);
    HASH_DEL(tabla[index], current_item);  /* delete; tabla advances to next */
    free(current_item);
  }
}

/***************************************************************************/

tablaSimbolos* crearTablaSimbolos(){
  tablaSimbolos* tabla;
  int i;
  tabla = (tablaSimbolos*)malloc(sizeof(tablaSimbolos));
  tabla->ambitoLocal = (entradaTabla**)malloc(sizeof(entradaTabla*)*MAX_LOCALS);
  for(i=0; i<MAX_LOCALS; i++){
    (tabla->ambitoLocal)[i] = NULL;
  }
  tabla->ambitoGlobal = (entradaTabla**)malloc(sizeof(entradaTabla*));
  *(tabla->ambitoGlobal) = NULL;
  return tabla;
}

void borrarTablaSimbolos(tablaSimbolos* tabla){
  int i;
  if(tabla->ambitoLocal){
    for(i=0; i<MAX_LOCALS; i++){
      delete_all((tabla->ambitoLocal), i);
    }
    free(tabla->ambitoLocal);

  }
  if(tabla->ambitoGlobal){
    delete_all(tabla->ambitoGlobal, 0);
    free(tabla->ambitoGlobal);
  }
  free(tabla);
}

void limpiarAmbitoLocal(tablaSimbolos* tabla, int index){
  delete_all(tabla->ambitoLocal, index);
  (tabla->ambitoLocal)[index] = NULL;
}

int buscarAmbitoLocal(tablaSimbolos* tabla, char* key, int index, int* value){
  entradaTabla* entrada;
  int i;
  if(index >= MAX_LOCALS || index < 0){
    return -1;
  }
  for(i=index; i>=0; i--){
    entrada = findValue(tabla->ambitoLocal, i, key);
    if(entrada != NULL){
      *value = entrada->value;
      return 1;
    }
  }
  entrada = findValue(tabla->ambitoGlobal, 0, key);
  if(entrada != NULL){
    *value = entrada->value;
    return 1;
  }
  return 0;
}

int buscarAmbitoGlobal(tablaSimbolos* tabla, char* key, int* value){
  entradaTabla* entrada;
  entrada = findValue(tabla->ambitoGlobal, 0, key);
  if(entrada != NULL){
    *value = entrada->value;
    return 1;
  }
  return 0;
}


int insertarAmbitoLocal(tablaSimbolos* tabla, char* key, int index, int value){
  if(index >= MAX_LOCALS || index < 0){
    return -1;
  }
  return addValue(tabla->ambitoLocal, key, index, value);
}

int insertarAmbitoGlobal(tablaSimbolos* tabla, char* key, int value){
  return addValue(tabla->ambitoGlobal, key, 0, value);
}
