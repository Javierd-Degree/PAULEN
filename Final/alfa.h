#ifndef ALFA_H
#define ALFA_H

#define MAX_LONG_ID 100
#define MAX_TAMANIO_VECTOR 64 

/* Categoría de un símbolo: variable, parámetro de función o función */
typedef enum { VARIABLE, PARAMETRO, FUNCION } CATEGORIA_SIMBOLO;

/* Tipo de un símbolo: sólo se trabajará con enteros y booleanos */
typedef enum { ENTERO, BOOLEANO } TIPO;

/* Categoría de la variable: puede ser variable atómicas (escalar) o lista/array (vector) */
typedef enum { ESCALAR, VECTOR } CATEGORIA;

/* Información de un símbolo */
typedef struct {
    char identificador [MAX_LONG_ID+1];                /* identificador */
    CATEGORIA_SIMBOLO cat_simbolo;      /* categoría del simbolo */
    TIPO tipo;                          /* tipo */
    CATEGORIA categoria;                /* categoria de la variable */
    /*int valor;*/                          /* valor si escalar */
    /*int longitud;*/                       /* longitud si vector */
    /*int num_parametros;*/                 /* número de parámetros si función */
    /*int posicion;*/                       /* posición en llamada a función si parámetro, posición de declaración si variable local de función */
    /*int num_var_locales;*/                /* número de variables locales si función */
    int info1;							/* valor si escalar, longitud si vector, num_parametros si funcion */
    int info2;							/* posición en llamada a función si parámetro, posición de declaración si variable local de función o num_var_locales si función */
} SIMBOLO;

typedef struct
{
 char lexema[MAX_LONG_ID+1];
 int tipo;
 int valor_entero;
 int es_direccion;
 int etiqueta;
}tipo_atributos;

#endif
