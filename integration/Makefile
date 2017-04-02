CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 

all: INTEG INTEGST INTEGSER

INTEG: ./obj/integ.o
	$(CC) ./obj/integ.o -o ./bin/integ $(CFLAGS)

./obj/integ.o: ./src/integ.c
	$(CC) -c ./src/integ.c -o ./obj/integ.o $(CFLAGS)

INTEGST: ./obj/integst.o
	$(CC) ./obj/integst.o -o ./bin/integst $(CFLAGS)

./obj/integst.o: ./src/integst.c
	$(CC) -c ./src/integst.c -o ./obj/integst.o $(CFLAGS)

INTEGSER: ./obj/integser.o
	$(CC) ./obj/integser.o -o ./bin/integser $(CFLAGS)

./obj/integser.o: ./src/integser.c
	$(CC) -c ./src/integser.c -o ./obj/integser.o $(CFLAGS)


clean:
	rm ./obj/*.o
