CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 -lm

all: COUNTSER COUNTPAR

COUNTSER: ./obj/counter.o
	$(CC) ./obj/counter.o -o ./bin/counter $(CFLAGS)

./obj/counter.o: ./src/counter.c
	$(CC) -c ./src/counter.c -o ./obj/counter.o -std=c99

COUNTPAR: ./obj/counterpar.o
	$(CC) ./obj/counterpar.o -o ./bin/counterpar $(CFLAGS)

./obj/counterpar.o: ./src/counterpar.c
	$(CC) -c ./src/counterpar.c -o ./obj/counterpar.o -std=c99 $(CFLAGS)

clean:
	rm ./obj/*.o
