CC = mpicc
CFLAGS = -std=c99

.PHONY: all COMPILE

all: COMPILE

COMPILE: ./obj/main.o ./obj/md5.o
	$(CC) ./obj/main.o ./obj/md5.o -o ./bin/main $(CFLAGS)

./obj/main.o: ./src/main.c
	$(CC) -c ./src/main.c -o ./obj/main.o $(CFLAGS)

./obj/md5.o: ./src/md5.c
	$(CC) -c ./src/md5.c -o ./obj/md5.o $(CFLAGS)

clean:
	rm -f ./obj/*.o
	rm -f ./bin/main
