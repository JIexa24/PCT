#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{

  long buffSize = atol(argv[1]);
  char resbuf[buffSize + 2], sendbuf[buffSize + 2];
  int rank,commsize;
  int i;

  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];

  double time = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  MPI_Get_processor_name(procname, &len);
  
  sendbuf[0] = '0' + rank/10;
  sendbuf[1] = '0' + rank%10;
  sendbuf[2] = '-';

  time = MPI_Wtime();
  if (rank == 0) {
    for (i = 1; i < commsize; i++) {
    MPI_Recv(&resbuf, buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  } else {
    MPI_Send(&sendbuf, buffSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }
  time = MPI_Wtime() - time;
  if (rank > 0) {
    printf("Process %d of %d on %s received message (%ld) from %s with time \t= %.6lf\n",rank,commsize,procname,buffSize,resbuf,time);
  }
//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


  MPI_Finalize();

  return 0;
}
