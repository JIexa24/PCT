#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{

  int buffSize = atoi(argv[1]);
  char resbuf[buffSize], sendbuf[buffSize];
  int rank,commsize;

  double time = 0;
  FILE * data = fopen("dataring.dat","a");
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  int prev = (rank - 1 + commsize) %commsize;
  int next = (rank + 1) % commsize;
  
  if (rank == 0)
    time = MPI_Wtime();

//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  MPI_Sendrecv(&sendbuf, buffSize, MPI_CHAR, next, 0, &resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  if (rank == 0) {
    time = MPI_Wtime() - time;
    fprintf(data,"Time %d = %.6lf\n colproc = %d\n\n",buffSize,time, commsize);
  }

  printf("Process %d received from %d\n",rank,prev);

  MPI_Finalize();
  return 0;
}
