#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{

  long buffSize = atol(argv[1]);
  
  int rank,commsize;
  int root = 0;

  char resbuf[buffSize];
  char sendbuf[buffSize];

  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];
 
  double time = 0;
  MPI_Comm comm;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  MPI_Get_processor_name(procname, &len);

  time = MPI_Wtime();

  if (rank == 0) { 
    MPI_Bcast(&sendbuf, buffSize, MPI_CHAR, root, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&resbuf, buffSize, MPI_CHAR, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  time = MPI_Wtime() - time;
  if (rank > 0) {
    printf("Process %d of %d on %s received message (%ld) from %d with time \t= %.6lf\n",rank,commsize,procname,buffSize,0,time);
  }
//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  MPI_Finalize();

  return 0;
}
