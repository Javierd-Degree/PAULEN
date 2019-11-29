CC = gcc
CFLAGS = -g -Wall
LIBS = uthash.h tablaSimbolos.o
EJS = prueba_tabla
######################################################################################################################################

all: $(EJS)

tablaSimbolos.o: tablaSimbolos.c tablaSimbolos.h
	$(CC) $(CFLAGS) -c tablaSimbolos.c

prueba_tabla: tablaSimbolos.o
	$(CC) $(CFLAGS) main.c $(LIBS) -o prueba_tabla

clean:
	rm -rf *.o $(EJS)
