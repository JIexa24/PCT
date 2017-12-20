CC = mpicc
CFLAGS = -g3 -O0 -std=c99 -lm

all: GEMV

GEMV: ./obj/DGEMV.o
	$(CC) ./obj/DGEMV.o -o ./bin/DGEMV $(CFLAGS)

./obj/DGEMV.o: ./src/DGEMV.c
	$(CC) -c ./src/DGEMV.c -o ./obj/DGEMV.o $(CFLAGS)

clean:
	rm ./obj/*.o
