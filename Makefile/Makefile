CC = gcc
CFLAGS = -Isrc -Wall
SRC = src/main.c src/lexer.c src/parser.c src/encoder.c src/linker.c

all: ensamblador

ensamblador:
	$(CC) $(CFLAGS) $(SRC) -o ensamblador.exe

run: all
	./ensamblador.exe examples/basico.asm

clean:
	rm -f ensamblador.exe
