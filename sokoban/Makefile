CC = gcc -std=c11 -O3 -Wall -Wpedantic -Wextra
DEL = rm -f
all:
	$(CC) *.c -c
	$(CC) game.o getch.o sokoban.o -o game
	$(CC) sokoban.o verifier.o -o verifier
	$(DEL) *.o
clean:
	$(DEL) game verifier
