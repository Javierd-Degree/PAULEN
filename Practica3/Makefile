CC = gcc
CFLAGS = -g -Wall
LIBS = tablaSimbolos.h uthash.h
EJS = prueba_tabla
######################################################################################################################################

all: $(EJS)

prueba_tabla:
	$(CC) $(CFLAGS) main.c $(LIBS) -o main.o

clean:
	rm -rf *.o $(EJS)
