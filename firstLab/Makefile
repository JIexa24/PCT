CC = mpicc
CFLAGS = -g3 -O0 -std=c99

all: mpi

mpi: ./obj/firstLabAlltoall.o ./obj/firstLabBroadcast.o ./obj/firstLabGather.o ./obj/firstLabRing.o
	$(CC) ./obj/firstLabAlltoall.o -o ./bin/firstLabAlltoall $(CFLAGS)
	$(CC) ./obj/firstLabBroadcast.o -o ./bin/firstLabBroadcast $(CFLAGS)
	$(CC) ./obj/firstLabGather.o -o ./bin/firstLabGather $(CFLAGS)
	$(CC) ./obj/firstLabRing.o -o ./bin/firstLabRing $(CFLAGS)

./obj/firstLabAlltoall.o: ./src/firstLabAlltoall.c
	$(CC) -c ./src/firstLabAlltoall.c -o ./obj/firstLabAlltoall.o $(CFLAGS)

./obj/firstLabBroadcast.o: ./src/firstLabBroadcast.c
	$(CC) -c ./src/firstLabBroadcast.c -o ./obj/firstLabBroadcast.o $(CFLAGS)

./obj/firstLabGather.o: ./src/firstLabGather.c
	$(CC) -c ./src/firstLabGather.c -o ./obj/firstLabGather.o $(CFLAGS)

./obj/firstLabRing.o: ./src/firstLabRing.c
	$(CC) -c ./src/firstLabRing.c -o ./obj/firstLabRing.o $(CFLAGS)

clean:
	rm ./obj/*.o
