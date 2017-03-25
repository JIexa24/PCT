CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 

all: DGEMV

DGEMV: ./obj/matrix.o
	$(CC) ./obj/matrix.o -o ./bin/matrix $(CFLAGS)

./obj/matrix.o: ./src/matrix.c
	$(CC) -c ./src/matrix.c -o ./obj/matrix.o $(CFLAGS)

clean:
	rm ./obj/*.o
