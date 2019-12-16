#include "generacion.h"

/*
   Código para el principio de la sección .bss.
   Con seguridad sabes que deberás reservar una variable entera para guardar el puntero de pila extendido (esp). Se te sugiere el nombre __esp para esta variable.
*/
void escribir_cabecera_bss(FILE* fpasm, tablaSimbolos* tabla){
  entradaTabla* entrada;
  fprintf(fpasm, "segment .bss\n");
  fprintf(fpasm, "__esp resd 1\n");

  for(entrada = *(tabla->ambitoGlobal); entrada != NULL; entrada = entrada->hh.next){
    SIMBOLO* simbolo;
    simbolo = entrada->value;
    if(simbolo->cat_simbolo == VARIABLE){
      if(simbolo->categoria){
        fprintf(fpasm, "__%s resd 1\n", simbolo->identificador);
      }else{
        fprintf(fpasm, "__%s resd %d\n", simbolo->identificador, simbolo->info1);
      }
    }
  }
}

/*
   Declaración (con directiva db) de las variables que contienen el texto de los mensajes para la identificación de errores en tiempo de ejecución.
   En este punto, al menos, debes ser capaz de detectar la división por 0.
*/
void escribir_subseccion_data(FILE* fpasm){
  fprintf(fpasm, "segment .data\n");
  fprintf(fpasm, "_division_by_cero_s db \"Division por cero\",0\n");
  fprintf(fpasm, "_out_of_range_s db \"Indice fuera de rango\",0\n");
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
  fprintf(fpasm, "extern scan_int, print_int, scan_boolean, print_boolean\n");
  fprintf(fpasm, "extern print_endofline, print_blank, print_string\n");
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
  fprintf(fpasm, "_end:\n");
  fprintf(fpasm, "mov dword esp, [__esp]\n");
  fprintf(fpasm, "ret\n");

  /* Implementamos el error de division por cero */
  fprintf(fpasm, "_division_by_cero:\n");
  fprintf(fpasm, "push dword _division_by_cero_s\n");
  fprintf(fpasm, "call print_string\n");
  fprintf(fpasm, "add esp, 4\n");
  fprintf(fpasm, "jmp _end\n");

  /* Implementamos el error de indice fuera de rango */
  fprintf(fpasm, "_out_of_range:\n");
  fprintf(fpasm, "push dword _out_of_range_s\n");
  fprintf(fpasm, "call print_string\n");
  fprintf(fpasm, "add esp, 4\n");
  fprintf(fpasm, "jmp _end\n");
}

/*
   Función que debe ser invocada cuando se sabe un operando de una operación aritmético-lógica y se necesita introducirlo en la pila.
-	nombre es la cadena de caracteres del operando tal y como debería aparecer en el fuente NASM
-	es_variable indica si este operando es una variable (como por ejemplo b1) con un 1 u otra cosa (como por ejemplo 34) con un 0. Recuerda que en el primer caso internamente se representará como _b1 y, sin embargo, en el segundo se representará tal y como esté en el argumento (34).
*/
void escribir_operando(FILE* fpasm, void* nombre, int es_variable){
  /*Si es un variable, guardamos la referencia a la variable _nombre*/
  if(es_variable==1){
    fprintf(fpasm, "push dword _%s\n", (char*)nombre);
  /*Si es un valor, guardamos el valor directamente en la pila*/
  }else{
    fprintf(fpasm, "push dword %d\n", *(int*)nombre);
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
    fprintf(fpasm, "mov [_%s], [eax]\n", nombre);
  /*Si no, guardamos en nombre el valor sacado de la pila*/
  }else{
    fprintf(fpasm, "mov [_%s], eax\n", nombre);
  }
}

/*
- Genera el código para asignar valor a la posición indexada de un vector cuya direccion está almacenada en la segunda posición de la pila.
- Se toma el valor de la cima de la pila.
- El último argumento es el que indica si lo que hay en la cima de la pila es una referencia (1) o ya un valor explícito (0).
*/
void asignar_vector(FILE* fpasm, int es_variable){
  /*sacamos el elemento superior de la pila*/
  fprintf(fpasm, "pop eax\n");
  /*Sacamos la dirección del vector ya indexado de la pila*/.
  fprintf(fpasm, "pop ebx\n");
  /*Si es una variable, guardamos en la variable nombre el contenido de la referencia sacada de la pila*/
  if(es_variable==1){
    fprintf(fpasm, "mov [ebx], [eax]\n");
  /*Si no, guardamos en nombre el valor sacado de la pila*/
  }else{
    fprintf(fpasm, "mov [ebx], eax\n");
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
   fprintf(fpasm, "pop dword ebx\n");
   if(es_variable_2 == 1){
      /* Hacemos onversion de referencia a valor */
      fprintf(fpasm, "mov ebx, [ebx]\n");
   }

   fprintf(fpasm, "pop dword eax\n");
   if(es_variable_1 == 1){
      /* Hacemos onversion de referencia a valor */
      fprintf(fpasm, "mov eax, [eax]\n");
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
   fprintf(fpasm, "push dword eax\n");
}


void dividir(FILE* fpasm, int es_variable_1, int es_variable_2){
   lectura_registros(fpasm, es_variable_1, es_variable_2);

   /* Extendemos el signo de eax para tener el dividendo en edx:eax */
   fprintf(fpasm, "cdq\n");
   /* Comprobamos division por cero y saltar al error si es necesario */
   fprintf(fpasm, "cmp ebx, 0\n");
   fprintf(fpasm, "je _division_by_cero\n");
   /* Si no ha habido division por cero, hacemos la division y guardamso el resultado */
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
   fprintf(fpasm, "jmp neg_fin_%d\n", cuantos_no);
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
  fprintf(fpasm, "equal_%d:\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "equal_end_%d:\n", etiqueta);
}

void distinto(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jne diff_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp diff_end_%d\n", etiqueta);
  fprintf(fpasm, "diff_%d:\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "diff_end_%d:\n", etiqueta);
}

void menor_igual(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jle less_eq_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp less_eq_end_%d\n", etiqueta);
  fprintf(fpasm, "less_eq_%d:\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "less_eq_end_%d:\n", etiqueta);
}
void mayor_igual(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jge more_eq_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp more_eq_end_%d\n", etiqueta);
  fprintf(fpasm, "more_eq_%d:\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "more_eq_end_%d:\n", etiqueta);
}
void menor(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jl less_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp less_end_%d\n", etiqueta);
  fprintf(fpasm, "less_%d:\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "less_end_%d:\n", etiqueta);
}

void mayor(FILE* fpasm, int es_variable_1, int es_variable_2, int etiqueta){
  lectura_registros(fpasm, es_variable_1, es_variable_2);
  fprintf(fpasm, "cmp eax, ebx\n");
  fprintf(fpasm, "jg more_%d\n", etiqueta);
  fprintf(fpasm, "push dword 0\n");
  fprintf(fpasm, "jmp more_end_%d\n", etiqueta);
  fprintf(fpasm, "more_%d:\n", etiqueta);
  fprintf(fpasm, "push dword 1\n");
  fprintf(fpasm, "more_end_%d:\n", etiqueta);
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

/* Generación de código para el inicio de una estructura if-then-else
Como es el inicio de uno bloque de control de flujo de programa que requiere de una nueva
etiqueta deben ejecutarse antes las tareas correspondientes a esta situación
exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)*/
void ifthenelse_inicio(FILE * fpasm, int exp_es_variable, int etiqueta){
	lectura_registro(fpasm, exp_es_variable);
	fprintf(fpasm, "cmp eax, 0\n");
	fprintf(fpasm, "je else_%d\n", etiqueta);
}

/* Generación de código para el inicio de una estructura if-then
Como es el inicio de uno bloque de control de flujo de programa que requiere de una nueva
etiqueta deben ejecutarse antes las tareas correspondientes a esta situación
exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)*/
void ifthen_inicio(FILE * fpasm, int exp_es_variable, int etiqueta){
	lectura_registro(fpasm, exp_es_variable);
	fprintf(fpasm, "cmp eax, 0\n");
	fprintf(fpasm, "je endif_%d\n", etiqueta);
}

/* Generación de código para el fin de una estructura if-then
Como es el fin de uno bloque de control de flujo de programa que hace uso de la etiqueta
del mismo se requiere que antes de su invocación tome el valor de la etiqueta que le toca
según se ha explicado
Y tras ser invocada debe realizar el proceso para ajustar la información de las etiquetas
puesto que se ha liberado la última de ellas.*/
void ifthen_fin(FILE * fpasm, int etiqueta){
	fprintf(fpasm, "endif_%d:\n", etiqueta);
}

/* Generación de código para el fin de la rama then de una estructura if-then-else
Sólo necesita usar la etiqueta adecuada, aunque es el final de una rama, luego debe venir
otra (la rama else) antes de que se termine la estructura y se tenga que ajustar las etiquetas
por lo que sólo se necesita que se utilice la etiqueta que corresponde al momento actual.*/
void ifthenelse_fin_then( FILE * fpasm, int etiqueta){
	fprintf(fpasm, "jmp endif_%d\n", etiqueta);
	fprintf(fpasm, "else_%d:\n", etiqueta);
}

/* Generación de código para el fin de una estructura if-then-else
Como es el fin de uno bloque de control de flujo de programa que hace uso de la etiqueta
del mismo se requiere que antes de su invocación tome el valor de la etiqueta que le toca
según se ha explicado
Y tras ser invocada debe realizar el proceso para ajustar la información de las etiquetas
puesto que se ha liberado la última de ellas*/
void ifthenelse_fin( FILE * fpasm, int etiqueta){
	fprintf(fpasm, "endif_%d:\n", etiqueta);
}

/* Generación de código para el inicio de una estructura while
Como es el inicio de uno bloque de control de flujo de programa que requiere de una nueva
etiqueta deben ejecutarse antes las tareas correspondientes a esta situación
exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)*/
void while_inicio(FILE * fpasm, int etiqueta){
	fprintf(fpasm, "while_%d:\n", etiqueta);
}

/* Generación de código para el momento en el que se ha generado el código de la expresión
de control del bucle
Sólo necesita usar la etiqueta adecuada, por lo que sólo se necesita que se recupere el valor
de la etiqueta que corresponde al momento actual.
/* exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
o elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)*/
void while_exp_pila (FILE * fpasm, int exp_es_variable, int etiqueta){
	lectura_registro(fpasm, exp_es_variable);
	fprintf(fpasm, "cmp eax, 0\n");
	fprintf(fpasm, "je whilend_%d\n", etiqueta);
}

/* Generación de código para el final de una estructura while
Como es el fin de uno bloque de control de flujo de programa que hace uso de la etiqueta
del mismo se requiere que antes de su invocación tome el valor de la etiqueta que le toca
según se ha explicado
Y tras ser invocada debe realizar el proceso para ajustar la información de las etiquetas
puesto que se ha liberado la última de ellas.*/
void while_fin( FILE * fpasm, int etiqueta){
  fprintf(fpasm, "jmp while_%d\n", etiqueta);
	fprintf(fpasm, "whilend_%d:\n", etiqueta);
}

/* Generación de código para indexar un vector
Cuyo nombre es nombre_vector
Declarado con un tamaño tam_max
La expresión que lo indexa está en la cima de la pila
Puede ser una variable (o algo equivalente) en cuyo caso exp_es_direccion vale 1.
Puede ser un valor concreto (en ese caso exp_es_direccion vale 0)
Según se especifica en el material, es suficiente con utilizar dos registros para realizar esta
tarea.*/
void escribir_elemento_vector(FILE * fpasm,char * nombre_vector, int tam_max, int exp_es_direccion){
	lectura_registro(fpasm, exp_es_direccion);
	/* Comprobamos que el indice esta en el rango adecuado */
	fprintf(fpasm, "cmp eax, 0\n");
	fprintf(fpasm, "jl _out_of_range\n");
	fprintf(fpasm, "cmp eax, %d\n", tam_max);
	fprintf(fpasm, "jge _out_of_range\n");

	fprintf(fpasm, "lea eax, [_%s + eax*4]\n", nombre_vector);
	fprintf(fpasm, "push dword eax\n");
}

/* Generación de código para iniciar la declaración de una función.
Es necesario proporcionar
Su nombre
Su número de variables locales*/
void declararFuncion(FILE * fpasm, char * nombre_funcion, int num_var_loc){
    fprintf(fpasm, "_%s:\n", nombre_funcion);
    fprintf(fpasm, "push ebp\n");
    fprintf(fpasm, "mov ebp, esp\n");
    fprintf(fpasm, "sub esp, %d\n", 4*num_var_loc);
}

/* Generación de código para el retorno de una función.
La expresión que se retorna está en la cima de la pila.
Puede ser una variable (o algo equivalente) en cuyo caso exp_es_direccion vale 1
Puede ser un valor concreto (en ese caso exp_es_direccion vale 0)*/
void retornarFuncion(FILE * fpasm, int es_variable){
    lectura_registro(fpasm, es_variable);
    fprintf(fpasm, "mov esp, ebp\n");
    fprintf(fpasm, "pop ebp\n");
    fprintf(fpasm, "ret\n");
}

/* Función para dejar en la cima de la pila la dirección efectiva del parámetro que ocupa la
posición pos_parametro (recuerda que los parámetros se ordenan con origen 0) de un total
de num_total_parametros*/
void escribirParametro(FILE* fpasm, int pos_parametro, int num_total_parametros){
    int index;
    /* Calculamos el indice para indexar en la pila */
    index = 4*(1 + (num_total_parametros - pos_parametro));

    fprintf(fpasm, "lea eax, [ebp + %d]\n", index);
    fprintf(fpasm, "push dword eax\n");
}

/* Función para dejar en la cima de la pila la dirección efectiva de la variable local que ocupa
la posición posicion_variable_local (recuerda que ordenadas con origen 1)*/
void escribirVariableLocal(FILE* fpasm, int posicion_variable_local){
    fprintf(fpasm, "lea eax, [ebp - %d]\n", 4*posicion_variable_local);
  	fprintf(fpasm, "push dword eax\n");
}

/* Función para poder asignar a un destino que no es una variable “global” (tipo _x) por
ejemplo parámetros o variables globales (ya que en ese caso su nombre real de alto nivel, no
se tiene en cuenta pues es realmente un desplazamiento a partir de ebp: ebp+4 o ebp-8 por
ejemplo.
Se debe asumir que en la pila estará
Primero (en la cima) lo que hay que asignar
Debajo (se ha introducido en la pila antes) la dirección donde hay que asignar
es_variable
Es 1 si la expresión que se va a asignar es algo asimilable a una variable (identificador, o elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)*/

void asignarDestinoEnPila(FILE* fpasm, int es_variable){
	fprintf(fpasm, "pop ebx\n");
  fprintf(fpasm, "pop eax\n");
  if(es_variable == 1){
     /* Hacemos onversion de referencia a valor */
     fprintf(fpasm, "mov eax, [eax]\n");
  }
  fprintf(fpasm, "mov dword [ebx], eax\n");


}

/* Como habrás visto en el material, nuestro convenio de llamadas a las funciones asume que
los argumentos se pasan por valor, esto significa que siempre se dejan en la pila “valores” y
no “variables”
Esta función realiza la tarea de dado un operando escrito en la pila y sabiendo si es variable
o no (es_variable) se deja en la pila el valor correspondiente*/
void operandoEnPilaAArgumento(FILE * fpasm, int es_variable){
  	lectura_registro(fpasm, es_variable);
  	fprintf(fpasm, "push dword eax\n");
}

/* Esta función genera código para llamar a la función nombre_funcion asumiendo que los
argumentos están en la pila en el orden fijado en el material de la asignatura.
Debe dejar en la cima de la pila el retorno de la función tras haberla limpiado de sus
argumentos
Para limpiar la pila puede utilizar la función de nombre limpiarPila*/
void llamarFuncion(FILE * fpasm, char * nombre_funcion, int num_argumentos){
	/* TODO comentar que en un lado se escribe la funcion con _ pero en el otro no*/
    fprintf(fpasm, "call _%s\n", nombre_funcion);
    limpiarPila(fpasm, num_argumentos);
    fprintf(fpasm, "push dword eax\n");
}

/* Genera código para limpiar la pila tras invocar una función
Esta función es necesaria para completar la llamada a métodos, su gestión dificulta el
conocimiento por parte de la función de llamada del número de argumentos que hay en la
pila*/
void limpiarPila(FILE * fpasm, int num_argumentos){
    fprintf(fpasm, "sub esp, %d\n", num_argumentos*4);
}
