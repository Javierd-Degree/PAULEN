CC = gcc
CFLAGS = -g -Wall
LIBS = lex.yy.c alfa.tab.c uthash.h alfa.h tablaSimbolos.o generacion.o
BISON_FLAGS = -ly -D_XOPEN_SOURCE=700
DEBUG = -DYYDEBUG=1
EJS = alfa
######################################################################################################################################

all: $(EJS)

tablaSimbolos.o: tablaSimbolos.c tablaSimbolos.h
	$(CC) $(CFLAGS) -c tablaSimbolos.c

generacion.o: generacion.c generacion.h
	$(CC) $(CFLAGS) -c generacion.c

alfa: tablaSimbolos.o generacion.o
	flex alfa.l
	bison -d alfa.y
	$(CC) $(CFLAGS) alfa.c $(LIBS) $(BISON_FLAGS) -o alfa

clean:
	rm -rf *.o tokens.h.gch alfa.tab.* lex* $(EJS)
