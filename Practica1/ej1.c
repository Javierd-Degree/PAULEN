#include <stdio.h>
#include "generacion.h"
#define MAX_ETIQUETAS 50
int main (int argc, char** argv)
{
  FILE * salida;
  int etiqueta = 0;
  int getiqueta = 0;
  int etiquetas[MAX_ETIQUETAS];
  int cima_etiquetas=-1;

  if (argc != 2) {
    fprintf (stdout, "ERROR POCOS ARGUMENTOS\n");
    return -1;
  }
  salida = fopen(argv[1],"w");
  escribir_subseccion_data(salida);
  escribir_cabecera_bss(salida);
  declarar_variable(salida, "b1", BOOLEANO, 1);
  declarar_variable(salida, "x", ENTERO, 1);
  escribir_segmento_codigo(salida);
  escribir_inicio_main(salida);
  /* b1 = true; */
  escribir_operando(salida, "1", 0);
  asignar(salida, "b1", 0);

  getiqueta++;
  cima_etiquetas++;
  etiquetas[cima_etiquetas] = getiqueta;

  etiqueta = etiquetas[cima_etiquetas];
  while_inicio(salida, etiqueta);
  escribir_operando(salida, "b1", 1);

  while_exp_pila(salida, 1, etiqueta);
  leer(salida, "x", ENTERO);
  escribir_operando(salida, "x", 1);
  escribir_operando(salida, "4", 0);

  getiqueta++;
  cima_etiquetas++;
  etiquetas[cima_etiquetas] = getiqueta;

  etiqueta = etiquetas[cima_etiquetas];
  mayor(salida, 1, 0, etiqueta);
  cima_etiquetas--;
  asignar(salida, "b1", 0);

  /* If */
  getiqueta++;
  cima_etiquetas++;
  etiquetas[cima_etiquetas] = getiqueta;

  etiqueta = etiquetas[cima_etiquetas];
  escribir_operando(salida, "b1", 1);
  ifthenelse_inicio(salida, 1, etiqueta);
  escribir_operando(salida, "x", 1);
  escribir(salida, 1, ENTERO);
  ifthenelse_fin_then(salida, etiqueta);
  escribir_operando(salida, "0", 0);
  escribir(salida, 0, BOOLEANO);
  ifthenelse_fin(salida, etiqueta);

  cima_etiquetas--;
  etiqueta = etiquetas[cima_etiquetas];
  while_fin(salida, etiqueta);

  cima_etiquetas--;

  /* Incluye aqui el codigo necesario para
  while (b1) {
    scanf x;
    b1 = x > 4;
    if (b1) {
      printf x;
    }
    else {
      printf false;
    }
  }
  */
  escribir_fin(salida);
  fclose(salida);
  return 0;
}
