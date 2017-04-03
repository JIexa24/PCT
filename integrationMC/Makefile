CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 -lm

all: INTEG INTEGSER

INTEG: ./obj/integ.o
	$(CC) ./obj/integ.o -o ./bin/integ $(CFLAGS)

./obj/integ.o: ./src/integ.c
	$(CC) -c ./src/integ.c -o ./obj/integ.o -std=c99

INTEGSER: ./obj/integser.o
	$(CC) ./obj/integser.o -o ./bin/integser $(CFLAGS)

./obj/integser.o: ./src/integser.c
	$(CC) -c ./src/integser.c -o ./obj/integser.o -std=c99


clean:
	rm ./obj/*.o
