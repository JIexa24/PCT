CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 

all: OPENMP MPI

OPENMP: MATRIX INTEGR

MATRIX:
	make -C DGEMV
 
INTEGR:
	make -C integration

MPI: FIRSTLAB

FIRSTLAB:
	make -C firstLab

clean:
	rm ./DGEMV/obj/*.o
	rm ./firstLab/obj/*.o
