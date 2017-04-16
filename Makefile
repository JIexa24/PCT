CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 

all: OPENMP MPI

OPENMP: MATRIX INTEGR

MATRIX:
	make -C DGEMV
 
INTEGR:
	make -C integration
	make -C integrationMC
	make -C jacobi


MPI: FIRSTLAB

FIRSTLAB:
	make -C firstLab

clean:
	rm ./DGEMV/obj/*.o
	rm ./firstLab/obj/*.o
	rm ./jacobi/obj/*.o
	rm ./integration/obj/*.o
	rm ./integrationMC/obj/*.o
