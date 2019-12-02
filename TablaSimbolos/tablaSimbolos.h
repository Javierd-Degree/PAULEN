#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

typedef struct _entradaTabla entradaTabla;

typedef struct _tablaSimbolos tablaSimbolos;

/****************************************************************************/

int addValue(entradaTabla** tabla, char* key, int value);

entradaTabla* findValue(entradaTabla** tabla, char* key);

void delete_all(entradaTabla** tabla);
/***************************************************************************/

tablaSimbolos* crearTablaSimbolos();

void borrarTablaSimbolos(tablaSimbolos* tabla);

void limpiarAmbitoLocal(tablaSimbolos* tabla);

int buscarAmbitoLocal(tablaSimbolos* tabla, char* key, int* value);

int buscarAmbitoGlobal (tablaSimbolos* tabla, char* key, int* value);

int insertarAmbitoLocal(tablaSimbolos* tabla, char* key, int value);

int insertarAmbitoGlobal(tablaSimbolos* tabla, char* key, int value);

#endif
