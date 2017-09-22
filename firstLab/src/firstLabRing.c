#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{

  long buffSize = atol(argv[1]);
  int rank,commsize;

  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];
 

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  MPI_Get_processor_name(procname, &len);
  
  char resbuf[buffSize], sendbuf[buffSize];
  int i = 0;  
  for (i = 0; i < buffSize - 1; i++) {
     sendbuf[i] = (rand() % ('z' - 'a') + 'a' + rank) % ('z' - 'a');
  }
  sendbuf[buffSize - 1] = '\0';
  int prev = (rank - 1 + commsize) %commsize;
  int next = (rank + 1) % commsize;
  
  double time = MPI_Wtime();

//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  MPI_Sendrecv(&sendbuf, buffSize, MPI_CHAR, next, 0, &resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  
  time = MPI_Wtime() - time;
    //fprintf(data,"Time %d = %.6lf\n colproc = %d\n\n",buffSize,time, commsize);
  printf("Process %d of %d on %s received message (%ld) from %d with time \t= %.6lf\n",rank,commsize,procname,buffSize,prev, time);


  MPI_Finalize();

  return 0;
}
