#include "generacion.h"

/*
   Código para el principio de la sección .bss.
   Con seguridad sabes que deberás reservar una variable entera para guardar el puntero de pila extendido (esp). Se te sugiere el nombre __esp para esta variable.
*/
void escribir_cabecera_bss(FILE* fpasm){
  fprintf(fpasm, "segment .bss\n");
  fprintf(fpasm, "__esp resd 1\n");
}

/*
   Declaración (con directiva db) de las variables que contienen el texto de los mensajes para la identificación de errores en tiempo de ejecución.
   En este punto, al menos, debes ser capaz de detectar la división por 0.
*/
void escribir_subseccion_data(FILE* fpasm){
  fprintf(fpasm, "segment .data\n");
}

/*
   Para ser invocada en la sección .bss cada vez que se quiera declarar una variable:
-	El argumento nombre es el de la variable.
-	tipo puede ser ENTERO o BOOLEANO (observa la declaración de las constantes del principio del fichero).
-	Esta misma función se invocará cuando en el compilador se declaren vectores, por eso se adjunta un argumento final (tamano) que para esta primera práctica siempre recibirá el valor 1.
*/
void declarar_variable(FILE* fpasm, char * nombre,  int tipo,  int tamano){
  /* Declaramos una variable de 32bits sin inicializar */
  fprintf(fpasm, "_%s resd %d\n", nombre, tamano);
}

/*
   Para escribir el comienzo del segmento .text, básicamente se indica que se exporta la etiqueta main y que se usarán las funciones declaradas en la librería olib.o
*/
void escribir_segmento_codigo(FILE* fpasm){
  fprintf(fpasm, "segment .text\n");
  fprintf(fpasm, "global main\n");
  fprintf(fpasm, "extern scan_int, print_int, scan_float, print_float, scan_boolean, print_boolean\n");
  fprintf(fpasm, "extern print_endofline, print_blank, print_string\n");
  fprintf(fpasm, "extern alfa_malloc, alfa_free, ld_float\n");
}

/*
   En este punto se debe escribir, al menos, la etiqueta main y la sentencia que guarda el puntero de pila en su variable (se recomienda usar __esp).
*/
void escribir_inicio_main(FILE* fpasm){
    fprintf(fpasm, "; -----------------------\n");
    fprintf(fpasm, "; PROCEDIMIENTO PRINCIPAL\n");
    fprintf(fpasm, "; -----------------------\n");
    fprintf(fpasm, "main:\n");
    fprintf(fpasm, "mov [__esp], esp\n");
}

/*
   Al final del programa se escribe:
-	El código NASM para salir de manera controlada cuando se detecta un error en tiempo de ejecución (cada error saltará a una etiqueta situada en esta zona en la que se imprimirá el correspondiente mensaje y se saltará a la zona de finalización del programa).
-	En el final del programa se debe:
         ·Restaurar el valor del puntero de pila (a partir de su variable __esp)
         ·Salir del programa (ret).
*/
void escribir_fin(FILE* fpasm){
  fprintf(fpasm, "mov dword esp, [__esp]\n");
  fprintf(fpasm, "ret\n");
}

/*
   Función que debe ser invocada cuando se sabe un operando de una operación aritmético-lógica y se necesita introducirlo en la pila.
-	nombre es la cadena de caracteres del operando tal y como debería aparecer en el fuente NASM
-	es_variable indica si este operando es una variable (como por ejemplo b1) con un 1 u otra cosa (como por ejemplo 34) con un 0. Recuerda que en el primer caso internamente se representará como _b1 y, sin embargo, en el segundo se representará tal y como esté en el argumento (34).
*/
void escribir_operando(FILE* fpasm, char* nombre, int es_variable){
  /*Si es un variable, guardamos la referencia a la variable _nombre*/
  if(es_variable==1){
    fprintf(fpasm, "push dword _%s\n", nombre);
  /*Si es un valor, guardamos el valor directamente en la pila*/
  }else{
    fprintf(fpasm, "push dword %s\n", nombre);
  }
}


/*
-	Genera el código para asignar valor a la variable de nombre nombre.
-	Se toma el valor de la cima de la pila.
-	El último argumento es el que indica si lo que hay en la cima de la pila es una referencia (1) o ya un valor explícito (0).
*/
void asignar(FILE* fpasm, char* nombre, int es_variable){
  /*sacamos el elemento superior de la pila*/
  fprintf(fpasm, "pop eax\n");
  /*Si es una variable, guardamos en la variable nombre el contenido de la referencia sacada de la pila*/
  if(es_variable==1){
    fprintf(fpasm, "mov %s, [eax]\n", nombre);
  /*Si no, guardamos en nombre el valor sacado de la pila*/
  }else{
    fprintf(fpasm, "mov %s, eax\n", nombre);
  }
}


/* FUNCIONES ARITMÉTICO-LÓGICAS BINARIAS */
/*
   En todas ellas se realiza la operación como se ha resumido anteriormente:
-	Se extrae de la pila los operandos
-	Se realiza la operación
-	Se guarda el resultado en la pila
   Los dos últimos argumentos indican respectivamente si lo que hay en la pila es una referencia a un valor o un valor explícito.
   Deben tenerse en cuenta las peculiaridades de cada operación. En este sentido sí hay que mencionar explícitamente que, en el caso de la división, se debe controlar si el divisor es “0” y en ese caso se debe saltar a la rutina de error controlado (restaurando el puntero de pila en ese caso y comprobando en el retorno que no se produce “Segmentation Fault”)
*/

/*
   Funcion auxiliar encargada de la lectura de un operando en registro eax desde la pila, bien sea por valor o por referencia.
*/
void lectura_registro(FILE* fpasm, int es_variable_1){
   fprintf(fpasm, "pop dword eax\n");
   if(es_variable_1 == 1){
      /* Hacemos onversion de referencia a valor */
      fprintf(fpasm, "mov eax, [eax]\n");
   }
}

/*
   Funcion auxiliar encargada de la lectura de los operandos en registros eax, ebx desde la pila, bien sea por valor o por referencia.
*/
void lectura_registros(FILE* fpasm, int es_variable_1, int es_variable_2){
   fprintf(fpasm, "pop dword eax\n");
   if(es_variable_1 == 1){
      /* Hacemos onversion de referencia a valor */
      fprintf(fpasm, "mov eax, [eax]\n");
   }

   fprintf(fpasm, "pop dword ebx\n");
   if(es_variable_2 == 1){
      /* Hacemos onversion de referencia a valor */
      fprintf(fpasm, "mov ebx, [ebx]\n");
   }
}


void sumar(FILE* fpasm, int es_variable_1, int es_variable_2){
   lectura_registros(fpasm, es_variable_1, es_variable_2);

   fprintf(fpasm, "add eax, ebx\n");
   fprintf(fpasm, "push dword eax\n");
}

void restar(FILE* fpasm, int es_variable_1, int es_variable_2){
   lectura_registros(fpasm, es_variable_1, es_variable_2);

   fprintf(fpasm, "sub eax, ebx\n");
   fprintf(fpasm, "push dword eax\n");
}

void multiplicar(FILE* fpasm, int es_variable_1, int es_variable_2){
   lectura_registros(fpasm, es_variable_1, es_variable_2);

   fprintf(fpasm, "imul ebx\n");
   /* Aunque el resultado se almacene en edx:eax, solo almacenamos eax */
   /* TODO: ¿Error por overflow ? */
   fprintf(fpasm, "push dword eax\n");
}


void dividir(FILE* fpasm, int es_variable_1, int es_variable_2){
   lectura_registros(fpasm, es_variable_1, es_variable_2);

   /* Extendemos el signo de eax para tener el dividendo en edx:eax */
   fprintf(fpasm, "cdq\n");
   /* TODO Comprobar division por cero y saltar al error si es necesario */
   fprintf(fpasm, "idiv ebx\n");
   fprintf(fpasm, "push dword eax\n");
}

void o(FILE* fpasm, int es_variable_1, int es_variable_2){
   lectura_registros(fpasm, es_variable_1, es_variable_2);

   fprintf(fpasm, "or eax, ebx\n");
   fprintf(fpasm, "push dword eax\n");
}

void y(FILE* fpasm, int es_variable_1, int es_variable_2){
   lectura_registros(fpasm, es_variable_1, es_variable_2);

   fprintf(fpasm, "and eax, ebx\n");
   fprintf(fpasm, "push dword eax\n");
}

/*
   Función aritmética de cambio de signo.
   Es análoga a las binarias, excepto que sólo requiere de un acceso a la pila ya que sólo usa un operando.
*/
void cambiar_signo(FILE* fpasm, int es_variable){
   lectura_registro(fpasm, es_variable);

   fprintf(fpasm, "neg eax\n");
   fprintf(fpasm, "push dword eax\n");
}

/*
   Función monádica lógica de negación. No hay un código de operación de la ALU que realice esta operación por lo que se debe codificar un algoritmo que, si encuentra en la cima de la pila un 0 deja en la cima un 1 y al contrario.
   El último argumento es el valor de etiqueta que corresponde (sin lugar a dudas, la implementación del algoritmo requerirá etiquetas). Véase en los ejemplos de programa principal como puede gestionarse el número de etiquetas cuantos_no.
*/
void no(FILE* fpasm, int es_variable, int cuantos_no){
   lectura_registro(fpasm, es_variable);

   /* Cargamos 0 en edx para hacer la comparacion */
   fprintf(fpasm, "mov edx, 0\n");
   /* Comparamos para ver si la variable tiene valor lógico cero */
   fprintf(fpasm, "cmp eax, edx\n");
   /* Si tiene valor lógico cero, saltamos para escribir un 1, si no, escribimos */
   fprintf(fpasm, "jz neg_cero_%d\n", cuantos_no);
   fprintf(fpasm, "mov eax, 0\n");
   /* Si hemos escrito un 0, saltamos al final del algoritmo*/
   fprintf(fpasm, "neg_fin_%d:\n", cuantos_no);
   /* Rama del if en la que se escribe el 1 */
   fprintf(fpasm, "neg_cero_%d:\n", cuantos_no);
   fprintf(fpasm, "mov eax, 1\n");
   /* Escribimos la etiqueta del fin del algoritmo */
   fprintf(fpasm, "neg_fin_%d:\n", cuantos_no);

   fprintf(fpasm, "push dword eax\n");
}

/* FUNCIONES COMPARATIVAS */
/*
   Todas estas funciones reciben como argumento si los elementos a comparar son o no variables. El resultado de las operaciones, que siempre será un booleano (“1” si se cumple la comparación y “0” si no se cumple), se deja en la pila como en el resto de operaciones. Se deben usar etiquetas para poder gestionar los saltos necesarios para implementar las comparaciones.
*/
void igual(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "je equal_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp equal_end_%d\n", etiqueta);
  fprintf(fpasm, "equal_%d\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "equal_end_%d\n", etiqueta);
}

void distinto(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jne fidd_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp diff_end_%d\n", etiqueta);
  fprintf(fpasm, "diff_%d\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "diff_end_%d\n", etiqueta);
}

void menor_igual(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jle less_eq_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp less_eq_end_%d\n", etiqueta);
  fprintf(fpasm, "less_eq_%d\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "less_eq_end_%d\n", etiqueta);
}
void mayor_igual(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jge more_eq_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp more_eq_end_%d\n", etiqueta);
  fprintf(fpasm, "more_eq_%d\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "more_eq_end_%d\n", etiqueta);
}
void menor(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jl less_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp less_end_%d\n", etiqueta);
  fprintf(fpasm, "less_%d\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "less_end_%d\n", etiqueta);
}

void mayor(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jg more_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp more_end_%d\n", etiqueta);
  fprintf(fpasm, "more_%d\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "more_end_%d\n", etiqueta);
}

/* FUNCIONES DE ESCRITURA Y LECTURA */
/*
   Se necesita saber el tipo de datos que se va a procesar (ENTERO o BOOLEANO) ya que hay diferentes funciones de librería para la lectura (idem. escritura) de cada tipo.
   Se deben insertar en la pila los argumentos necesarios, realizar la llamada (call) a la función de librería correspondiente y limpiar la pila.
*/
void leer(FILE* fpasm, char* nombre, int tipo){
  fprintf(fpasm, "push dword _%s\n", nombre);
  if(tipo == BOOLEANO){
    fprintf(fpasm, "call scan_boolean\n");
  }else if(tipo == ENTERO){
    fprintf(fpasm, "call scan_int\n");
  }
  if(tipo == ENTERO||BOOLEANO){
    fprintf(fpasm, "add esp, 4\n");
  }
}

void escribir(FILE* fpasm, int es_variable, int tipo){
  lectura_registro(fpasm, es_variable);
  fprintf(fpasm, "push dword eax\n");
  if(tipo == BOOLEANO){
    fprintf(fpasm, "call print_boolean\n");
  }else if(tipo == ENTERO){
    fprintf(fpasm, "call print_int\n");
  }
  if(tipo == ENTERO||BOOLEANO){
    fprintf(fpasm, "add esp, 4\n");
  }

  /* Pasamos a una linea nueva */
  fprintf(fpasm, "call print_endofline\n");
}


void suma_iterativa(FILE *fpasm, char *nombre1, char *nombre2){
  /* Leemos las dos variables */
  leer(fpasm, nombre1, ENTERO);
  leer(fpasm, nombre2, ENTERO);

  /* Cargamos las variables en los registros */
  fprintf(fpasm, "mov dword ecx, [_%s]\n", nombre1);
  fprintf(fpasm, "mov dword ebx, [_%s]\n", nombre2);

  /* Iniciamos el bucle while */
  fprintf(fpasm, "while:\n");
  fprintf(fpasm, "cmp ebx, 0\n");
  fprintf(fpasm, "jz fin\n");
  /* Sumamos las dos variables */
  fprintf(fpasm, "add ecx, ebx\n");

  /* Imprimimos el resultado de la operación y leemos Y de nuevo */
  fprintf(fpasm, "push dword ecx\n");
  escribir(fpasm, 0, ENTERO);
  leer(fpasm, nombre2, ENTERO);

  fprintf(fpasm, "mov dword ebx, [_%s]\n", nombre2);
  /* Volvemos a la condición del bucle while */
  fprintf(fpasm, "jmp while\n");
  /* Etiqueta que denota el final de la rutina */
  fprintf(fpasm, "fin:\n");
}



