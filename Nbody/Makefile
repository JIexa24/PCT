CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 -lm

all: V1 V2 V3 SER

SER: ./obj/serial.o
	$(CC) ./obj/serial.o -o ./bin/serial $(CFLAGS)

./obj/serial.o: ./src/serial.c
	$(CC) -c ./src/serial.c -o ./obj/serial.o -std=c99

V1: ./obj/v1.o
	$(CC) ./obj/v1.o -o ./bin/v1 $(CFLAGS)

./obj/v1.o: ./src/v1.c
	$(CC) -c ./src/v1.c -o ./obj/v1.o -std=c99

V2: ./obj/v2.o
	$(CC) ./obj/v2.o -o ./bin/v2 $(CFLAGS)

./obj/v2.o: ./src/v2.c
	$(CC) -c ./src/v2.c -o ./obj/v2.o -std=c99

V3: ./obj/v3.o
	$(CC) ./obj/v3.o -o ./bin/v3 $(CFLAGS)

./obj/v3.o: ./src/v3.c
	$(CC) -c ./src/v3.c -o ./obj/v3.o -std=c99

clean:
	rm ./obj/*.o
