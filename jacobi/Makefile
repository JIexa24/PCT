CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 -lm

all: JAC JACP1 JACP2

JAC: ./obj/jacobiser.o
	$(CC) ./obj/jacobiser.o -o ./bin/jacobiser $(CFLAGS)

./obj/jacobiser.o: ./src/jacobiser.c
	$(CC) -c ./src/jacobiser.c -o ./obj/jacobiser.o -std=c99

JACP1: ./obj/jacobipar1.o
	$(CC) ./obj/jacobipar1.o -o ./bin/jacobipar1 $(CFLAGS)

./obj/jacobipar1.o: ./src/jacobipar1.c
	$(CC) -c ./src/jacobipar1.c -o ./obj/jacobipar1.o -std=c99

JACP2: ./obj/jacobipar2.o
	$(CC) ./obj/jacobipar2.o -o ./bin/jacobipar2 $(CFLAGS)

./obj/jacobipar2.o: ./src/jacobipar2.c
	$(CC) -c ./src/jacobipar2.c -o ./obj/jacobipar2.o -std=c99

clean:
	rm ./obj/*.o
