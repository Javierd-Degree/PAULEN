#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uthash.h"
#include "tablaSimbolos.h"

int getLocal(tablaSimbolos* tabla){
  return tabla->isAmbitoLocal;
}
/****************************************************************************/

entradaTabla* crearEntrada(int len){
  entradaTabla* entrada;
  entrada = (entradaTabla*)malloc(sizeof(entradaTabla));
  entrada->key = (char*)malloc(sizeof(char)*len);
  entrada->value = NULL;
  return entrada;
}

int addValue(entradaTabla** tabla, char* key, SIMBOLO* value){
  entradaTabla* entrada;
  HASH_FIND_STR(*tabla, key, entrada);
  if (entrada == NULL) {
    entrada = crearEntrada(strlen(key)+1);
    strcpy(entrada->key, key);
    entrada->value = value;
    HASH_ADD_STR(*tabla, key, entrada);
    return 1;
  }
  return 0;
}

entradaTabla* findValue(entradaTabla** tabla, char* key) {
    entradaTabla* value;

    if(*tabla == NULL){
      return NULL;
    }

    HASH_FIND_STR(*tabla, key, value );  /* value: output pointer */
    return value;
}

void delete_all(entradaTabla** tabla) {
  entradaTabla *current_item, *tmp;

  HASH_ITER(hh, *tabla, current_item, tmp) {
    /*al eliminar la key eliminamos tambien el identificador de la estructura SIMBOLO*/
    free(current_item->key);
    free(current_item->value);
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

void iniciarAmbitoLocal(tablaSimbolos* tabla){
  tabla->isAmbitoLocal = 1;
}

void limpiarAmbitoLocal(tablaSimbolos* tabla){
  tabla->isAmbitoLocal = 0;
  delete_all(tabla->ambitoLocal);
  free(tabla->ambitoLocal);
  tabla->ambitoLocal = (entradaTabla**)malloc(sizeof(entradaTabla*));
  *(tabla->ambitoLocal) = NULL;
}

int buscarAmbitoLocal(tablaSimbolos* tabla, char* key, SIMBOLO** value){
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

int buscarAmbitoGlobal(tablaSimbolos* tabla, char* key, SIMBOLO** value){
  entradaTabla* entrada;
  entrada = findValue(tabla->ambitoGlobal, key);
  if(entrada != NULL){
    *value = entrada->value;
    return 1;
  }
  return 0;
}

int buscar(tablaSimbolos* tabla, char* key, SIMBOLO** value){
  if(tabla->isAmbitoLocal == 0){
    return buscarAmbitoGlobal(tabla, key, value);
  }
  return buscarAmbitoLocal(tabla, key, value);
}

int insertarAmbitoLocal(tablaSimbolos* tabla, char* key, SIMBOLO* value){
  return addValue(tabla->ambitoLocal, key, value);
}

int insertarAmbitoGlobal(tablaSimbolos* tabla, char* key, SIMBOLO* value){
  return addValue(tabla->ambitoGlobal, key, value);
}

int insertar(tablaSimbolos* tabla, char* key, SIMBOLO* value){
  if(tabla->isAmbitoLocal == 0){
    return insertarAmbitoGlobal(tabla, key, value);
  }
  return insertarAmbitoLocal(tabla, key, value);
}
