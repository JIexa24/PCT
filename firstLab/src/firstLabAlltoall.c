#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
  long buffSize = atol(argv[1]);
  int rank,commsize;
  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];
  char *recvbuf;
  char *sendbuf;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);
  MPI_Request req[2];
  recvbuf = malloc(sizeof(char) * buffSize * (commsize - 1));
  sendbuf = malloc(sizeof(char) * buffSize);

  int i = 0;
  for (i = 0; i < buffSize - 1; i++) {
     sendbuf[i] = (rand() % ('z' - 'a') + 'a' + rank) % ('z' - 'a');
  }
  sendbuf[buffSize - 1] = '\0';

  double time = MPI_Wtime();
  for (i = 0; i < commsize; i++) {
    if (i == rank) continue;
      MPI_Isend(&sendbuf, buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, &(req[0]));
      MPI_Irecv(&(recvbuf[(i > rank ? i - 1 : i) * buffSize]), buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, &(req[1]));
      MPI_Waitall(2, req, MPI_STATUS_IGNORE);
  }
/*  MPI_Alltoall(sendbuf, buffSize, MPI_CHAR,
               recvbuf, buffSize, MPI_CHAR, MPI_COMM_WORLD);  */
  time = MPI_Wtime() - time;

  printf("Process %d of %d on %s(\n%s\n%s\n) received message (%ld) with time \t= %.6lf\n", rank, commsize, procname,sendrecvbuf, buffSize, time);

  //MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
  //MPI_Recv(&recvbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  free(sendbuf);
  free(recvbuf);

  MPI_Finalize();

  return 0;
}
