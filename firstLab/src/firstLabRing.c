#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
  long buffSize = atol(argv[1]);
  int rank, commsize;
  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];
  char *recvbuf, *sendbuf;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);
  recvbuf = (char*)malloc(sizeof(char) * buffSize);
  sendbuf = (char*)malloc(sizeof(char) * buffSize);

  int i = 0;
  for (i = 0; i < buffSize - 1; i++) {
     sendbuf[i] = (rand() % ('z' - 'a') + 'a' + rank) % ('z' - 'a');
  }
  sendbuf[buffSize - 1] = '\0';
  int prev = (rank - 1 + commsize) % commsize;
  int next = (rank + 1) % commsize;

  //MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
  //MPI_Recv(&recvbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  double time = MPI_Wtime();
  MPI_Sendrecv(sendbuf, buffSize, MPI_CHAR, next, 0, recvbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  time = MPI_Wtime() - time;

  int flag = 0;
  for (i = 0; i < buffSize; i++) {
     if (sendbuf[i] != recvbuf[i]) {
       flag = 1;
       break;
     }
  }
  //fprintf(data,"Time %d = %.6lf\n colproc = %d\n\n",buffSize,time, commsize);
  printf("Process %d of %d on %s(%d) received message (%ld) from %d with time \t= %.6lf\n",rank,commsize,procname, flag,buffSize,prev, time);

  free(sendbuf);
  free(recvbuf);

  MPI_Finalize();

  return 0;
}
