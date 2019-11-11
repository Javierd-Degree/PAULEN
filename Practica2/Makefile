CC = gcc
CFLAGS = -g -Wall
LIBS = lex.yy.c tokens.h
EJS = main
######################################################################################################################################

all: $(EJS)

main:
	$(CC) $(CFLAGS) main.c $(LIBS) -o main.o

clean:
	rm -rf *.o $(EJS)
