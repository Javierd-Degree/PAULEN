CC = gcc
CFLAGS = -g -Wall
LIBS = lex.yy.c alfa.tab.c tokens.h
BISON_FLAGS = -ly -D_XOPEN_SOURCE=700
DEBUG = -DYYDEBUG=1
EJS = pruebaSintactico
######################################################################################################################################

all: $(EJS)

pruebaSintactico:
	flex alfa.l
	bison -d alfa.y
	$(CC) $(CFLAGS) main.c $(LIBS) $(BISON_FLAGS) -o pruebaSintactico

clean:
	rm -rf *.o tokens.h.gch alfa.tab.* lex* $(EJS)
