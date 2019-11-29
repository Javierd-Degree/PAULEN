CC = gcc
CFLAGS = -g -Wall
LIBS = lex.yy.c alfa.tab.c tokens.h
BISON_FLAGS = -ly -D_XOPEN_SOURCE=700
DEBUG = -DYYDEBUG=1
EJS = main
######################################################################################################################################

all: $(EJS)

main:
	flex alfa.l
	bison -d alfa.y
	$(CC) $(CFLAGS) main.c $(LIBS) $(BISON_FLAGS) -o main

clean:
	rm -rf *.o tokens.h.gch alfa.tab.* lex* $(EJS)
