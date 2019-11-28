
#include "uthash.h"
#include "tablaSimbolos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 256

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

entradaTabla* crearEntrada(){
  entradaTabla* entrada;
  entrada = (entradaTabla*)malloc(sizeof(entradaTabla));
  entrada->key = (char*)malloc(sizeof(char)*MAX_LINE);
  return entrada;
}

int addValue(entradaTabla** tabla, char* key, int value){
  entradaTabla* entrada;
  HASH_FIND_STR(*tabla, key, entrada);
  if (entrada == NULL) {
    entrada = crearEntrada();
    strcpy(entrada->key, key);
    entrada->value = value;
    HASH_ADD_STR(*tabla, key, entrada);
    return 1;
  }
  return 0;
}

entradaTabla* findValue(entradaTabla** tabla, char* key) {
    entradaTabla* value;

    if(tabla == NULL){
      return NULL;
    }

    HASH_FIND_STR(*tabla, key, value );  /* value: output pointer */
    return value;
}

void deleteItem(entradaTabla** tabla, entradaTabla* item) {
    HASH_DEL(*tabla, item);  /* item: pointer to deletee */
    free(item);
}

void delete_all(entradaTabla** tabla) {
  entradaTabla *current_item, *tmp;

  HASH_ITER(hh, *tabla, current_item, tmp) {
    free(current_item->key);
    HASH_DEL(*tabla, current_item);  /* delete; tabla advances to next */
    free(current_item);
  }
}

/***************************************************************************/

tablaSimbolos* crearTablaSimbolos(){
  tablaSimbolos* tabla;
  tabla = (tablaSimbolos*)malloc(sizeof(tablaSimbolos));
  tabla->ambitoLocal = (entradaTabla**)malloc(sizeof(entradaTabla*));
  *(tabla->ambitoLocal) = NULL;
  tabla->ambitoGlobal = (entradaTabla**)malloc(sizeof(entradaTabla*));
  *(tabla->ambitoGlobal) = NULL;
  return tabla;
}

void borrarTablaSimbolos(tablaSimbolos* tabla){
  if(tabla->ambitoLocal){
    delete_all(tabla->ambitoLocal);
    free(tabla->ambitoLocal);
  }
  if(tabla->ambitoGlobal){
    delete_all(tabla->ambitoGlobal);
    free(tabla->ambitoGlobal);
  }
  free(tabla);
}

void limpiarAmbitoLocal(tablaSimbolos* tabla){
  delete_all(tabla->ambitoLocal);
  free(tabla->ambitoLocal);
  tabla->ambitoLocal = (entradaTabla**)malloc(sizeof(entradaTabla*));
  *(tabla->ambitoLocal) = NULL;
}

int buscarAmbitoLocal(tablaSimbolos* tabla, char* key, int* value){
  entradaTabla* entrada;
  entrada = findValue(tabla->ambitoLocal, key);
  if(entrada != NULL){
    *value = entrada->value;
    return 1;
  }
  entrada = findValue(tabla->ambitoGlobal, key);
  if(entrada != NULL){
    *value = entrada->value;
    return 1;
  }
  return 0;
}

int buscarAmbitoGlobal(tablaSimbolos* tabla, char* key, int* value){
  entradaTabla* entrada;
  entrada = findValue(tabla->ambitoGlobal, key);
  if(entrada != NULL){
    *value = entrada->value;
    return 1;
  }
  return 0;
}


int insertarAmbitoLocal(tablaSimbolos* tabla, char* key, int value){
  return addValue(tabla->ambitoLocal, key, value);
}

int insertarAmbitoGlobal(tablaSimbolos* tabla, char* key, int value){
  return addValue(tabla->ambitoGlobal, key, value);
}
