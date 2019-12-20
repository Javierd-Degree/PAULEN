#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

#include "alfa.h"
#include "uthash.h"

struct _entradaTabla{
  char* key;
  SIMBOLO* value;
  UT_hash_handle hh;
};
typedef struct _entradaTabla entradaTabla;

struct _tablaSimbolos{
  entradaTabla** ambitoLocal;
  entradaTabla** ambitoGlobal;
  int isAmbitoLocal;
};
typedef struct _tablaSimbolos tablaSimbolos;

int getLocal(tablaSimbolos* tabla);
/****************************************************************************/

int addValue(entradaTabla** tabla, char* key, SIMBOLO* value);

entradaTabla* findValue(entradaTabla** tabla, char* key);

void delete_all(entradaTabla** tabla);
/***************************************************************************/

tablaSimbolos* crearTablaSimbolos();

void borrarTablaSimbolos(tablaSimbolos* tabla);

void iniciarAmbitoLocal(tablaSimbolos* tabla);

void limpiarAmbitoLocal(tablaSimbolos* tabla);

int buscar(tablaSimbolos* tabla, char* key, SIMBOLO** value);

int insertar(tablaSimbolos* tabla, char* key, SIMBOLO* value);

#endif
