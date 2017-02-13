#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{

  long buffSize = atol(argv[1]);
  char *resbuf, *sendbuf;
  int rank,commsize;

  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];

  double time = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  MPI_Get_processor_name(procname, &len);
  

  resbuf = malloc(sizeof(char) * commsize * buffSize);
  sendbuf = malloc(sizeof(char) * commsize * buffSize);

  time = MPI_Wtime();
  MPI_Alltoall(sendbuf, buffSize, MPI_CHAR,
               resbuf,buffSize, MPI_CHAR, MPI_COMM_WORLD); 
  time = MPI_Wtime() - time;

    printf("Process %d of %d on %s received message (%ld) with time \t= %.6lf\n",rank,commsize,procname,buffSize,time);

//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  free(sendbuf);
  free(resbuf);
  MPI_Finalize();

  return 0;
}
