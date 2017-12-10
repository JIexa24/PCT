CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 

all: OPENMP MPI

OPENMP: MATRIX INTEGR COUNTER NBODY
 
MATRIX:
	make -C DGEMV
 
COURSE:
	make -C MPI_Course_Work_Gaussian_Determinant
	make -C MPI_Course_Work_DecodeMd5
	
INTEGR:
	make -C integration
	make -C integrationMC
	make -C jacobi

COUNTER:
	make -C counter

NBODY:
	make -C Nbody

MPI: FIRSTLAB MPI_INTEG

FIRSTLAB:
	make -C firstLab

MPI_INTEG:
	make -C MPI_integration
	make -C MPI_integration


clean:
	rm ./DGEMV/obj/*.o
	rm ./firstLab/obj/*.o
	rm ./jacobi/obj/*.o
	rm ./integration/obj/*.o
	rm ./integrationMC/obj/*.o
