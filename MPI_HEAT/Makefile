CC = mpicc
CFLAGS = -g3 -O0 -std=c99 -lm

all: HEAT1D HEAT2D 

HEAT1D: ./obj/MPI_HEAT_1D.o
	$(CC) ./obj/MPI_HEAT_1D.o -o ./bin/MPI_HEAT_1D $(CFLAGS)

./obj/MPI_HEAT_1D.o: ./src/MPI_HEAT_1D.c
	$(CC) -c ./src/MPI_HEAT_1D.c -o ./obj/MPI_HEAT_1D.o -std=c99

HEAT2D: ./obj/MPI_HEAT_2D.o
	$(CC) ./obj/MPI_HEAT_2D.o -o ./bin/MPI_HEAT_2D $(CFLAGS)

./obj/MPI_HEAT_2D.o: ./src/MPI_HEAT_2D.c
	$(CC) -c ./src/MPI_HEAT_2D.c -o ./obj/MPI_HEAT_2D.o -std=c99


clean:
	rm ./obj/*.o
