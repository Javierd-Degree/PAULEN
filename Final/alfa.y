%{
	#include <stdio.h>
	#include "tablaSimbolos.h"
	#include "alfa.h"
   #include "generacion.h"

   FILE* out;
	tablaSimbolos *tabla;
   TIPO tipo_actual, tipo_ret;
   CATEGORIA clase_actual;
   CATEGORIA_SIMBOLO cat_simbolo;
   int tamanio_vector_actual;
   int pos_variable_local_actual=1;
   int num_var_locales = 0;
   int pos_parametro = 1;
   int num_parametros=0;
   int num_parametros_act=0;
   int en_funcion = 0;
   int ret = 0;

   int etiqueta = 0;

   int yylex();
   void yyerror(const char *s);

   extern long nline, ncolumn;
   extern int yymorph;
   extern int yyleng;

%}

%union
{
	tipo_atributos atributos;
}

%start programa

%token TOK_MAIN
%token TOK_INT
%token TOK_BOOLEAN
%token TOK_ARRAY
%token TOK_FUNCTION
%token TOK_IF
%token TOK_ELSE
%token TOK_WHILE
%token TOK_SCANF
%token TOK_PRINTF
%token TOK_RETURN


%token TOK_PUNTOYCOMA
%token TOK_COMA
%token TOK_PARENTESISIZQUIERDO
%token TOK_PARENTESISDERECHO
%token TOK_CORCHETEIZQUIERDO
%token TOK_CORCHETEDERECHO
%token TOK_LLAVEIZQUIERDA
%token TOK_LLAVEDERECHA
%token TOK_ASIGNACION
%token TOK_MAS
%token TOK_MENOS
%token TOK_DIVISION
%token TOK_ASTERISCO
%token TOK_AND
%token TOK_OR
%token TOK_NOT
%token TOK_IGUAL
%token TOK_DISTINTO
%token TOK_MENORIGUAL
%token TOK_MAYORIGUAL
%token TOK_MENOR
%token TOK_MAYOR

%token TOK_ERROR

%token<atributos> TOK_IDENTIFICADOR
%token<atributos> TOK_CONSTANTE_ENTERA
%token<atributos> TOK_TRUE
%token<atributos> TOK_FALSE

%left TOK_MAS TOK_MENOS TOK_OR
%left TOK_ASTERISCO TOK_DIVISION TOK_AND
%right TOK_NOT


%type <atributos> exp
%type <atributos> comparacion
%type <atributos> clase_vector
%type <atributos> funcion
%type <atributos> fn_declaration
%type <atributos> fn_name
%type <atributos> asignacion
%type <atributos> elemento_vector
%type <atributos> if_check
%type <atributos> if_else_sentencias
%type <atributos> bucle
%type <atributos> while_check
%type <atributos> inicio_while
%type <atributos> constante
%type <atributos> idf_llamada_funcion
%type <atributos> constante_logica
%type <atributos> constante_entera

%%

programa: TOK_MAIN TOK_LLAVEIZQUIERDA declaraciones data_segment funciones main_segment sentencias TOK_LLAVEDERECHA {
   fprintf(out, ";R1:\t<programa> ::= main { <declaraciones> <funciones> <sentencias> }\n");
   escribir_fin(out);
};

data_segment: {
   fprintf(out, ";escribir_subseccion_data\n");
   escribir_subseccion_data(out);
   fprintf(out, ";escribir_cabecera_bss\n");
   escribir_cabecera_bss(out, tabla);
   fprintf(out, ";escribir_segmento_codigo\n");
   escribir_segmento_codigo(out);
}

main_segment: {
   fprintf(out, ";escribir_inicio_main\n");
   escribir_inicio_main(out);
}

declaraciones: declaracion {
   fprintf(out, ";R2:\t<declaraciones> ::= <declaracion>\n");
} | declaracion declaraciones {
   fprintf(out, ";R3:\t<declaraciones> ::= <declaracion> <declaraciones>\n");
}

declaracion: clase identificadores TOK_PUNTOYCOMA {
   fprintf(out, ";R4:\t<declaracion> ::= <clase> <identificadores> ;\n");
};

clase: clase_escalar {
   fprintf(out, ";R5:\t<clase> ::= <clase_escalar>\n");
   clase_actual = ESCALAR;
} | clase_vector {
   fprintf(out, ";R7:\t<clase> ::= <clase_vector>\n");
   clase_actual = VECTOR;
}

clase_escalar: tipo {
   fprintf(out, ";R9:\t<clase_escalar> ::= <tipo>\n");
};

clase_vector: TOK_ARRAY tipo TOK_CORCHETEIZQUIERDO constante_entera TOK_CORCHETEDERECHO {
   fprintf(out, ";R15:\t<clase_vector> ::= array <tipo> [<constante_entera>]\n");
   tamanio_vector_actual = $4.valor_entero;
   if(tamanio_vector_actual < 1 || tamanio_vector_actual > 64){
      printf("****Error semantico en lin %ld: El tamanyo del vector excede los limites permitidos (1,64).", nline);
      return -1;
   }
};

tipo: TOK_INT {
   fprintf(out, ";R10:\t<tipo> ::= int\n");
   tipo_actual = ENTERO;
} | TOK_BOOLEAN {
   fprintf(out, ";R11:\t<tipo> ::= boolean\n");
   tipo_actual = BOOLEANO;
};

identificadores: identificador {
   fprintf(out, ";R18:\t<identificadores> ::= <identificador>\n");
} | identificador TOK_COMA identificadores {
   fprintf(out, ";R19:\t<identificadores> ::= <identificador> , <identificadores>\n");
}

funciones: funcion funciones {
   fprintf(out, ";R20:\t<funciones> ::= <funcion>, <funciones>\n");
} | {
   fprintf(out, ";R21:\t<funciones> ::=\n");
};

funcion: fn_declaration sentencias TOK_LLAVEDERECHA {
   fprintf(out, ";R22:\t<funcion> ::= function <tipo> <identificador> ( <parametros_funcion> ){<declaraciones_funcion> <setencias>}\n");
   limpiarAmbitoLocal(tabla);
   SIMBOLO* simbolo;
   buscar(tabla, $1.lexema, &simbolo);
   simbolo->info1 = num_parametros;
   if(ret == 0){
      printf("****Error semantico en lin %ld: Funcion %s sin sentencia de retorno.", nline, $1.lexema);
      return -1;
   }
};

fn_declaration: fn_name TOK_PARENTESISIZQUIERDO parametros_funcion TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA declaraciones_funcion {
   SIMBOLO* simbolo;
   en_funcion = 0;
   buscar(tabla, $1.lexema, &simbolo);
   simbolo->info1 = num_parametros;
   strcpy($$.lexema, $1.lexema);
   $$.tipo = $1.tipo;
   ret = 0;
   declararFuncion(out, $1.lexema, num_var_locales);
}

fn_name: TOK_FUNCTION tipo TOK_IDENTIFICADOR {

   SIMBOLO* simbolo, *simbolo_copia;
   simbolo = (SIMBOLO*)malloc(sizeof(SIMBOLO));
   strcpy(simbolo->identificador, $3.lexema);
   simbolo->cat_simbolo=FUNCION;
   simbolo->tipo=tipo_actual;
   simbolo->categoria=ESCALAR;
   simbolo->info1=0;
   simbolo->info2=0;
   if(insertar(tabla, $3.lexema, simbolo) == 0){
      printf("****Error semantico en lin %ld: Declaracion duplicada.", nline);
      return -1;
   }
   iniciarAmbitoLocal(tabla);
   simbolo_copia = (SIMBOLO*)malloc(sizeof(SIMBOLO));
   strcpy(simbolo_copia->identificador, $3.lexema);
   simbolo_copia->cat_simbolo=FUNCION;
   simbolo_copia->tipo=tipo_actual;
   simbolo_copia->categoria=ESCALAR;
   simbolo_copia->info1=0;
   simbolo_copia->info2=0;
   insertar(tabla, $3.lexema, simbolo_copia);
   pos_variable_local_actual = 0;
   pos_parametro = 0;
   num_var_locales = 0;
   num_parametros = 0;
   strcpy($$.lexema, $3.lexema);
   $$.tipo = tipo_actual;
   tipo_ret = tipo_actual;
   en_funcion = 1;
};

parametros_funcion: parametro_funcion resto_parametros_funcion {
   fprintf(out, ";R23\t<parametros_funcion> ::= <parametro_funcion> <resto_parametros_funcion>\n");
} | {
   fprintf(out, ";R24\t<parametros_funcion> ::=\n");
};

resto_parametros_funcion:TOK_PUNTOYCOMA parametro_funcion resto_parametros_funcion {
   fprintf(out, ";R25\t<resto_parametros_funcion> ::= ; <parametro_funcion> <resto_parametros_funcion>\n");
} | {
   fprintf(out, ";R26\t<resto_parametros_funcion> ::=\n");
};

parametro_funcion: tipo idpf {
   fprintf(out, ";R27:\t<parametro_funcion> ::= <tipo> <identificador>\n");
   pos_parametro++;
   num_parametros ++;
};

declaraciones_funcion: declaraciones {
   fprintf(out, ";R28:\t<declaraciones_funcion ::= <declaraciones>\n");
} | {
   fprintf(out, ";R29:\t<declaraciones_funcion ::=\n");
};

sentencias: sentencia {
   fprintf(out, ";R30:\t<sentencias> ::= <sentencia>\n");
} | sentencia sentencias {
   fprintf(out, ";R31:\t<sentencias> ::= <sentencia> <sentencias>\n");
};

sentencia: sentencia_simple TOK_PUNTOYCOMA {
   fprintf(out, ";R32:\t<sentencia> ::= <sentencia_simple> ;\n");
} | bloque {
   fprintf(out, ";R33:\t<sentencia> ::= <bloque>\n");
};

sentencia_simple: asignacion {
   fprintf(out, ";R34:\t<sentencia_simple> ::= <asignacion>\n");
} | lectura {
   fprintf(out, ";R35:\t<sentencia_simple> ::= <lectura>\n");
} | escritura {
   fprintf(out, ";R36:\t<sentencia_simple> ::= <escritura>\n");
} | retorno_funcion {
   fprintf(out, ";R38:\t<sentencia_simple> ::= <retorno_funcion>\n");
};

bloque: condicional {
   fprintf(out, ";R40:\t<bloque> ::= <condicional>\n");
} | bucle {
   fprintf(out, ";R41:\t<bloque> ::= <bucle>\n");
};


asignacion: TOK_IDENTIFICADOR TOK_ASIGNACION exp {
   fprintf(out, ";R43:\t<asignacion> ::= <identificador> = <exp>\n");
   SIMBOLO* simbolo;
   int res;
   res = buscar(tabla, $1.lexema, &simbolo);
   if(res == 0){
      printf("****Error semantico en lin %ld: Acceso a variable no declarada %s.", nline, $1.lexema);
      return -1;
   }
   if(simbolo->cat_simbolo == FUNCION){
      printf("****Error semantico en lin %ld: Asignacion incompatible.", nline);
      return -1;
   }
   if(simbolo->categoria == VECTOR){
      printf("****Error semantico en lin %ld: Asignacion incompatible.", nline);
      return -1;
   }
   if(simbolo->tipo != $3.tipo){
      printf("****Error semantico en lin %ld: Asignacion incompatible.", nline);
      return -1;
   }
   if(res == 2){
      escribirVariableLocal(out, simbolo->info2);
      asignarDestinoEnPila(out, $3.es_direccion);
   }else{
      asignar(out, $1.lexema, $3.es_direccion);
   }
} | elemento_vector TOK_ASIGNACION exp {
   fprintf(out, ";R44:\t<asignacion> ::= <elemento_vector> = <exp>\n");
   if($1.tipo != $3.tipo){
      printf("****Error semantico en lin %ld: Asignacion incompatible.", nline);
      return -1;
   }
   escribir_operando(out, &$1.valor_entero, 0);
   escribir_elemento_vector(out, $1.lexema, tamanio_vector_actual, $3.es_direccion);
   asignarDestinoEnPila(out, $3.es_direccion);
};

elemento_vector: TOK_IDENTIFICADOR TOK_CORCHETEIZQUIERDO exp TOK_CORCHETEDERECHO {
   fprintf(out, ";R48:\t<elemento_vector> ::= <identificador> [<exp>]\n");
   SIMBOLO* simbolo;
   if(buscar(tabla, $1.lexema, &simbolo) == 0){
      printf("****Error semantico en lin %ld: Acceso a variable no declarada %s.", nline, $1.lexema);
      return -1;
   }
   if(simbolo->categoria != VECTOR){
      printf("****Error semantico en lin %ld: Intento de indexacion de una variable que no es de tipo vector.", nline);
      return -1;
   }
   if($3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: El indice en una operacion de indexacion tiene que ser de tipo entero.", nline);
      return -1;
   }
   $$.tipo = simbolo->tipo;
   $$.es_direccion = 1;
   $$.valor_entero = $3.valor_entero;
   escribir_elemento_vector(out, $1.lexema, simbolo->info1 , $3.es_direccion);
};

condicional: if_else_sentencias TOK_LLAVEDERECHA{
   fprintf(out, ";R50:\t<condicional> ::= if ( <exp> ) { <sentencias> }\n");
   ifthenelse_fin(out, $1.etiqueta);
} | if_else_sentencias TOK_LLAVEDERECHA TOK_ELSE TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA{
   fprintf(out, ";R51:\t<condicional> ::= if ( <exp> ) { <sentencias> } esle { <setencias> }\n");
   ifthenelse_fin(out, $1.etiqueta);
}

if_check: TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA{
   if($3.tipo != BOOLEANO){
      printf("****Error semantico en lin %ld: Condicional con condicion de tipo int.", nline);
      return -1;
   }
   $$.etiqueta = etiqueta++;
   ifthen_inicio(out, $3.es_direccion, $$.etiqueta);
}

if_else_sentencias: if_check sentencias {
   $$.etiqueta = $1.etiqueta;
   ifthenelse_fin_then(out, $$.etiqueta);
}

bucle: while_check TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA {
   fprintf(out, ";R52:\t<bucle> ::= while ( <exp> ) { <sentencias> }\n");
   while_fin(out, $1.etiqueta);
};

while_check: inicio_while TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO {
   if($3.tipo != BOOLEANO){
      printf("****Error semantico en lin %ld: Bucle con condicion de tipo int.", nline);
      return -1;
   }
   $$.etiqueta = $1.etiqueta;
   while_exp_pila (out, $3.es_direccion, $1.etiqueta);
}

inicio_while: TOK_WHILE {
   $$.etiqueta = etiqueta++;
   while_inicio(out, $$.etiqueta);
}

lectura: TOK_SCANF TOK_IDENTIFICADOR {
   fprintf(out, ";R54:\t<lectura> ::= scanf <identificador>\n");
   SIMBOLO* simbolo;
   if(buscar(tabla, $2.lexema, &simbolo) == 0){
      printf("****Error semantico en lin %ld: Acceso a variable no declarada %s.", nline, $2.lexema);
      return -1;
   }

   if(simbolo->categoria != ESCALAR){
      printf("****Error semantico en lin %ld: Asignacion incompatible.", nline);
      return -1;
   }
   leer(out, $2.lexema, simbolo->tipo);
};

escritura: TOK_PRINTF exp {
   fprintf(out, ";R56:\t<escritura> ::= printf <exp>\n");
   escribir(out, $2.es_direccion, $2.tipo);
};

retorno_funcion: TOK_RETURN exp {
   fprintf(out, ";R61:\t<retorno_funcion> ::= return <exp>\n");
   ret ++;
   if(tipo_ret != $2.tipo){
      printf("****Error semantico en lin %ld: Tipo de retorno de la funcion incorrecto.", nline);
      return -1;
   }
   retornarFuncion(out, $2.es_direccion);
};

exp: exp TOK_MAS exp {
   fprintf(out, ";R72:\t<exp> ::= <exp> + <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Operacion aritmetica con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = ENTERO;
   $$.es_direccion = 0;
   sumar(out, $1.es_direccion, $3.es_direccion);
} | exp TOK_MENOS exp {
   fprintf(out, ";R73:\t<exp> ::= <exp> - <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Operacion aritmetica con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = ENTERO;
   $$.es_direccion = 0;
   restar(out, $1.es_direccion, $3.es_direccion);
} | exp TOK_DIVISION exp {
   fprintf(out, ";R74:\t<exp> ::= <exp> / <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Operacion aritmetica con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = ENTERO;
   $$.es_direccion = 0;
   dividir(out, $1.es_direccion, $3.es_direccion);
} | exp TOK_ASTERISCO exp {
   fprintf(out, ";R75:\t<exp> ::= <exp> * <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Operacion aritmetica con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = ENTERO;
   $$.es_direccion = 0;
   multiplicar(out, $1.es_direccion, $3.es_direccion);
} | TOK_MENOS exp {
   fprintf(out, ";R76:\t<exp> ::= - <exp>\n");
   if($2.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Operacion aritmetica con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = ENTERO;
   $$.es_direccion = 0;
   cambiar_signo(out, $2.es_direccion);
} | exp TOK_AND exp {
   fprintf(out, ";R77:\t<exp> ::= <exp> && <exp>\n");
   if($1.tipo != BOOLEANO || $3.tipo != BOOLEANO){
      printf("****Error semantico en lin %ld: Operacion logica con operandos int.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   y(out, $1.es_direccion, $3.es_direccion);
} | exp TOK_OR exp {
   fprintf(out, ";R78:\t<exp> ::= <exp> || <exp>\n");
   if($1.tipo != BOOLEANO || $3.tipo != BOOLEANO){
      printf("****Error semantico en lin %ld: Operacion logica con operandos int.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   o(out, $1.es_direccion, $3.es_direccion);
} | TOK_NOT exp {
   fprintf(out, ";R79:\t<exp> ::= ! <exp>\n");
   if($2.tipo != BOOLEANO){
      printf("****Error semantico en lin %ld: Operacion logica con operandos int.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   no(out, $2.es_direccion, etiqueta);
   etiqueta ++;
} | TOK_IDENTIFICADOR {
   fprintf(out, ";R80:\t<exp> ::= <identificador>\n");
   SIMBOLO* simbolo;
   int res;
   res = buscar(tabla, $1.lexema, &simbolo);
   if(res == 0){
      printf("****Error semantico en lin %ld: Acceso a variable no declarada %s.", nline, $1.lexema);
      return -1;
   }
   if(simbolo->cat_simbolo == FUNCION){
      printf("****Error semantico en lin %ld: Numero incorrecto de parametros en llamada a funcion.", nline);
      return -1;
   }
   if(simbolo->categoria == VECTOR){
      printf("****Error semantico en lin %ld: No se puede usar un vector sin indexar.", nline);
      return -1;
   }
   $$.tipo = simbolo->tipo;
   $$.es_direccion = 1;
   if(simbolo->cat_simbolo == PARAMETRO){
      escribirParametro(out, simbolo->info2, num_parametros);
   }else if(simbolo->cat_simbolo == VARIABLE){
      if(res == 2){
         escribirVariableLocal(out, simbolo->info2);
      }else{
         escribir_operando(out, simbolo->identificador, 1);
         if(en_funcion == 1){
            operandoEnPilaAArgumento(out, 1);
         }
      }
   }
} | constante {
   fprintf(out, ";R81:\t<exp> ::= <constante>\n");
   $$.tipo = $1.tipo;
   $$.es_direccion = $1.es_direccion;
} | TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO {
   fprintf(out, ";R82:\t<exp> ::= ( <exp> )\n");
   $$.tipo = $2.tipo;
   $$.es_direccion = $2.es_direccion;
} | TOK_PARENTESISIZQUIERDO comparacion TOK_PARENTESISDERECHO {
   fprintf(out, ";R83:\t<exp> ::= ( <comparacion> )\n");
   $$.tipo = $2.tipo;
   $$.es_direccion = $2.es_direccion;
} | elemento_vector {
   fprintf(out, ";R85:\t<exp> ::= <elemento_vector>\n");
   $$.tipo = $1.tipo;
   $$.es_direccion = $1.es_direccion;
   if(en_funcion != 0){
      operandoEnPilaAArgumento(out, 1);
   }
} | idf_llamada_funcion TOK_PARENTESISIZQUIERDO lista_expresiones TOK_PARENTESISDERECHO {
   fprintf(out, ";R88:\t<exp> ::= <identificador> ( <lista_expresiones> )\n");
   SIMBOLO* simbolo;
   buscar(tabla, $1.lexema, &simbolo);
   if(simbolo->info1 != num_parametros_act){
      printf("****Error semantico en lin %ld: Numero incorrecto de parametros en llamada a funcion.", nline);
      return -1;
   }
   llamarFuncion(out, $1.lexema, simbolo->info1);
   en_funcion = 0;
   $$.tipo = simbolo->tipo;
   $$.es_direccion = 0;
};

idf_llamada_funcion: TOK_IDENTIFICADOR {
   SIMBOLO* simbolo;
   if(buscar(tabla, $1.lexema, &simbolo) == 0){
      printf("****Error semantico en lin %ld: Acceso a variable no declarada %s.", nline, $1.lexema);
      return -1;
   }
   if(simbolo->cat_simbolo != FUNCION){
      printf("****Error semantico en lin %ld: %s no es una funcion.", nline, simbolo->identificador);
      return -1;
   }
   if(en_funcion != 0){
      printf("****Error semantico en lin %ld: No esta permitido el uso de llamadas a funciones como parametros de otras funciones.", nline);
      return -1;
   }
   num_parametros_act = 0;
   en_funcion = 1;
   strcpy($$.lexema, simbolo->identificador);
}

lista_expresiones: exp resto_lista_expresiones {
   fprintf(out, ";R89:\t<lista_expresiones> ::= <exp> <resto_lista_expresiones>\n");
   num_parametros_act ++;
} | {
   fprintf(out, ";R90:\t<lista_expresiones> ::= \n");
};

resto_lista_expresiones: TOK_COMA exp resto_lista_expresiones {
   fprintf(out, ";R91:\t<resto_lista_expresiones> ::= , <exp> <resto_lista_expresiones>\n");
   num_parametros_act ++;
} | {
   fprintf(out, ";R92:\t<resto_lista_expresiones> ::=\n");
};

comparacion: exp TOK_IGUAL exp {
   fprintf(out, ";R93:\t<comparacion> ::= <exp> == <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Comparacion con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   igual(out, $1.es_direccion, $3.es_direccion, etiqueta);
   etiqueta ++;
} | exp TOK_DISTINTO exp {
   fprintf(out, ";R94:\t<comparacion> ::= <exp> != <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Comparacion con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   distinto(out, $1.es_direccion, $3.es_direccion, etiqueta);
   etiqueta ++;
} | exp TOK_MENORIGUAL exp {
   fprintf(out, ";R95:\t<comparacion> ::= <exp> <= <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Comparacion con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   menor_igual(out, $1.es_direccion, $3.es_direccion, etiqueta);
   etiqueta ++;
} | exp TOK_MAYORIGUAL exp {
   fprintf(out, ";R96:\t<comparacion> ::= <exp> >= <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Comparacion con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   mayor_igual(out, $1.es_direccion, $3.es_direccion, etiqueta);
   etiqueta ++;
} | exp TOK_MENOR exp {
   fprintf(out, ";R97:\t<comparacion> ::= <exp> < <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Comparacion con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   menor(out, $1.es_direccion, $3.es_direccion, etiqueta);
   etiqueta ++;
} | exp TOK_MAYOR exp {
   fprintf(out, ";R98:\t<comparacion> ::= <exp> > <exp>\n");
   if($1.tipo != ENTERO || $3.tipo != ENTERO){
      printf("****Error semantico en lin %ld: Comparacion con operandos boolean.", nline);
      return -1;
   }
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   mayor(out, $1.es_direccion, $3.es_direccion, etiqueta);
   etiqueta ++;
};

constante: constante_entera {
   fprintf(out, ";R99:\t<constante> ::= <constante_entera\n");
   $$.tipo = $1.tipo;
   $$.es_direccion = $1.es_direccion;
} | constante_logica {
   fprintf(out, ";R100:\t<constante> ::= <constante_logica\n");
   $$.tipo = $1.tipo;
   $$.es_direccion = $1.es_direccion;
};

constante_logica: TOK_TRUE {
   fprintf(out, ";R102:\t<constante_logica> ::= true\n");
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   escribir_operando(out, &($1.valor_entero), 0);
} | TOK_FALSE {
   fprintf(out, ";R103:\t<constante_logica> ::= false\n");
   $$.tipo = BOOLEANO;
   $$.es_direccion = 0;
   escribir_operando(out, &($1.valor_entero), 0);
};


constante_entera: TOK_CONSTANTE_ENTERA {
   fprintf(out, ";R104:\t<constante_entera> ::= TOK_CONSTANTE_ENTERA\n");
   $$.tipo = ENTERO;
   $$.es_direccion = 0;
   escribir_operando(out, &($1.valor_entero), 0);
};

identificador: TOK_IDENTIFICADOR {
   fprintf(out, ";R108:\t<identificador> ::= TOK_IDENTIFICADOR\n");
   if(clase_actual == VECTOR){
      if(tamanio_vector_actual<1||tamanio_vector_actual>MAX_TAMANIO_VECTOR){
         printf("****Error semantico en lin %ld: El tamanyo del vector %s excede los limites permitidos (1,64).", nline, $1.lexema);
         return -1;
      }
   }
   if(getLocal(tabla) == 1){
      if(clase_actual==VECTOR){
         printf("****Error semantico en lin %ld:Variable local de tipo no escalar.", nline);
         return -1;
      }
      pos_variable_local_actual++;
      num_var_locales++;
   }

   SIMBOLO* simbolo;
   simbolo = (SIMBOLO*)malloc(sizeof(SIMBOLO));
   strcpy(simbolo->identificador, $1.lexema);
   simbolo->cat_simbolo=VARIABLE;
   simbolo->tipo=tipo_actual;
   simbolo->categoria=clase_actual;
   if(clase_actual == ESCALAR){
      simbolo->info1=$1.valor_entero;
   } else{
      simbolo->info1=tamanio_vector_actual;
   }
   simbolo->info2=pos_variable_local_actual;
   if(insertar(tabla, $1.lexema, simbolo) == 0){
      printf("****Error semantico en lin %ld: Declaracion duplicada.", nline);
      return -1;
   }
};

idpf: TOK_IDENTIFICADOR {
   SIMBOLO* simbolo;
   simbolo = (SIMBOLO*)malloc(sizeof(SIMBOLO));
   strcpy(simbolo->identificador, $1.lexema);
   simbolo->cat_simbolo=PARAMETRO;
   simbolo->tipo=tipo_actual;
   simbolo->categoria=ESCALAR;
   simbolo->info1=0;
   simbolo->info2=pos_parametro;
   if(insertar(tabla, $1.lexema, simbolo) == 0){
      printf("****Error semantico en lin %ld: Declaracion duplicada.", nline);
      return -1;
   }
};
%%

void yyerror(const char *s) {
  if (!yymorph){
    printf("****Error sintactico en [lin %ld, col %ld]\n", nline, ncolumn-yyleng);
  }
}
