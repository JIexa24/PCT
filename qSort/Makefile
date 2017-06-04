CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 -lm

all: SER PAR

SER: ./obj/main_serial.o
	$(CC) ./obj/main_serial.o -o ./bin/main_serial $(CFLAGS)

./obj/main_serial.o: ./src/main_serial.c
	$(CC) -c ./src/main_serial.c -o ./obj/main_serial.o -std=c99

PAR: ./obj/main_parallel.o
	$(CC) ./obj/main_parallel.o -o ./bin/main_parallel $(CFLAGS)

./obj/main_parallel.o: ./src/main_parallel.c
	$(CC) -c ./src/main_parallel.c -o ./obj/main_parallel.o  $(CFLAGS)

clean:
	rm ./obj/*.o
