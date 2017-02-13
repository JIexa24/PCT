#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{

  long buffSize = atol(argv[1]);
  char *resbuf, sendbuf[buffSize];
  int rank,commsize;
  int root = 0;

  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];

  double time = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  MPI_Get_processor_name(procname, &len);
  
  time = MPI_Wtime();
  if (rank == 0) {
    resbuf = malloc(sizeof(char) * commsize * buffSize);
  }

  MPI_Gather(sendbuf, buffSize, MPI_CHAR, resbuf, buffSize, MPI_CHAR, root, MPI_COMM_WORLD);

  time = MPI_Wtime() - time;
  if (rank > 0) {
    printf("Process %d of %d on %s received message (%ld) with time \t= %.6lf\n",rank,commsize,procname,buffSize,time);
  }
//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  if (rank == 0)
  free(resbuf);
  MPI_Finalize();

  return 0;
}
