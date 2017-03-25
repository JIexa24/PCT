CC = gcc
CFLAGS = -g3 -O0 -fopenmp -std=c99 

all: MATRIX

MATRIX:
	make -C DGEMV
 
clean:
	rm ./DGEMV/obj/*.o
