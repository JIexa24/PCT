CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 -lm

all: JAC JACP 

JAC: ./obj/jacobiser.o
	$(CC) ./obj/jacobiser.o -o ./bin/jacobiser $(CFLAGS)

./obj/jacobiser.o: ./src/jacobiser.c
	$(CC) -c ./src/jacobiser.c -o ./obj/jacobiser.o -std=c99

JACP: ./obj/jacobipar.o
	$(CC) ./obj/jacobipar.o -o ./bin/jacobipar $(CFLAGS)

./obj/jacobipar.o: ./src/jacobipar.c
	$(CC) -c ./src/jacobipar.c -o ./obj/jacobipar.o -std=c99

clean:
	rm ./obj/*.o
