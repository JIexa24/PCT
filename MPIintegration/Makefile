CC = mpicc
CFLAGS = -g3 -O0 -std=c99 -lm

all: INTEG 

INTEG: ./obj/integ.o
	$(CC) ./obj/integ.o -o ./bin/integ $(CFLAGS)

./obj/integ.o: ./src/integ.c
	$(CC) -c ./src/integ.c -o ./obj/integ.o -std=c99

clean:
	rm ./obj/*.o
