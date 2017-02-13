#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{

  long buffSize = atol(argv[1]);
  char resbuf[buffSize], sendbuf[buffSize];
  int rank,commsize;
  int i;

  double time = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  time = MPI_Wtime();
  if (rank == 0) {
    for (i = 1; i < commsize; i++) {
      MPI_Send(&sendbuf, buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(&resbuf, buffSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  time = MPI_Wtime() - time;
  if (rank > 0) {
    printf("Process %d of %d received message (%ld) from %d with time \t= %.6lf\n",rank,commsize,buffSize,0,time);
  }
//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


  MPI_Finalize();

  return 0;
}
