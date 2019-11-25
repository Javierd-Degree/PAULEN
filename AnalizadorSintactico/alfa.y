%{

%}

%%

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

%token TOK_IDENTIFICADOR


%token TOK_CONSTANTE_ENTERA
%token TOK_TRUE
%token TOK_FALSE

%%

programa: TOK_MAIN TOK_LLAVEIZQUIERDA declaraciones funciones sentencias TOK_LLAVEDERECHA
{fprintf(out, ";R1\t<programa> ::= main { <declaraciones> <funciones> <sentencias> }\n")};

declaraciones: declaracion {fprintf(out, ";R2:\t<declaraciones> ::= <declaracion>\n");}
             | declaracion declaraciones {fprintf(out, ";R3:\t<declaraciones> ::= <declaracion> <declaraciones>\n");}

decalarcion: clase identificadores {fprintf(out, ";R4:\t<decalarcion> ::= <clase> <identificadores>\n");};
clase: clase_escalar {fprintf(out, ";R5:\t<clase> ::= <clase_escalar>\n");}
     | clase_vector {fprintf(out, ";R7:\t<clase> ::= <clase_vector>\n");}
clase_escalar: tipo {fprintf(out, ";R9:\t<clase_escalar> ::= <tipo>\n");};
tipo: TOK_INT {fprintf(out, ";R10:\t<tipo> ::= int\n");}
    | TOK_BOOLEAN {fprintf(out, ";R11:\t<tipo> ::= boolean\n");};
clase_vector: TOK_ARRAY TOK_CORCHETEIZQUIERDO constante_entera TOK_CORCHETEDERECHO {fprintf(out, ";R15:\t<clase_vector> ::= array <tipo> [<constante_entera>]\n");};
identificadores: identificador {fprintf(out, ";R18:\t<identificadores> ::= <identificador>\n");}
               | identificador TOK_COMA identificadores {fprintf(out, ";R19:\t<identificadores> ::= <identificador>, <identificadores>\n");}
funciones: funcion funciones {fprintf(out, ";R19:\t<funciones> ::= <funcion>, <funciones>\n");}
          | /*vacio*/;
funcion: tipo identificador TOK_PARENTESISIZQUIERDO parametros_funcion TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA declaraciones_funcion sentencias TOK_LLAVEDERECHA
         {fprintf(out, ";22:\t<funcion> ::= function <tipo> <identificador> ( <parametros_funcion> ){<declaraciones_funcion> <setencias>}\n");};
parametros_funcion: parametro_funcion resto_parametros_funcion;
resto_parametros_funcion:TOK_PUNTOYCOMA parametro_funcion resto_parametros_funcion
                        | /*Vacio*/;
parametro_funcion: tipo identificador;

declaraciones_funcion: declaraciones;
sentencias: sentencia
            | sentencia sentencias;
sentencia: sentencia_simpe
           | bloque;
sentencia_simpe: asignacion
                | lectura
                | escritura
                | retorno_funcion;
bloque: condicional
        | bucle;

asignacion: identificador TOK_ASIGNACION exp
            | elemento_vector TOK_ASIGNACION exp;
elemento_vector: identificador TOK_CORCHETEIZQUIERDO exp TOK_CORCHETEDERECHO;
condicional: TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA
            | TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA TOK_LLAVEIZQUIERDA else TOK_LLAVEDERECHA;
bucle: TOK_WHILE TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA;
lectura: TOK_SCANF identificador;
escritura: TOK_PRINTF exp;
retorno_funcion: TOK_RETURN exp;
exp: exp TOK_MAS exp
   | exp TOK_MENOS exp
   | exp TOK_DIVISION exp
   | exp TOK_ASTERISCO exp
   | TOK_MENOS exp
   | exp TOK_AND exp
   | exp TOK_OR exp
   | TOK_NOT exp
   | identificador
   | constante
   | TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO
   | TOK_PARENTESISIZQUIERDO comparacion TOK_PARENTESISDERECHO
   | elemento vector
   | identificador TOK_PARENTESISIZQUIERDO lista_expresiones TOK_PARENTESISDERECHO;
lista_expresiones: exp resto_lista_expresiones;
resto_lista_expresiones: TOK_COMA exp resto_lista_expresiones
                        | /*Vacio*/;
comparacion: exp TOK_IGUAL exp
           | exp TOK_DISTINTO exp
           | exp TOK_MENORIGUAL exp
           | exp TOK_MAYORIGUAL exp
           | exp TOK_MENOR exp
           | exp TOK_MAYOR exp;

constante: constante_logica
          | constante_entera;
constante_logica: TOK_TRUE
                | TOK_FALSE;
constante_entera: numero;
numero: digito
      | numero digito;
identificador: TOK_IDENTIFICADOR;
