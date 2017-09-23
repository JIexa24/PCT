#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
  long buffSize = atol(argv[1]);
  int rank,commsize;
  int root = 0;
  int len;
  char* sendresbuf;
  char procname[MPI_MAX_PROCESSOR_NAME];

  MPI_Comm comm;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);

  sendresbuf = malloc(sizeof(char)* buffSize);
  if (rank == 0) {
    int i = 0;
    for (i = 0; i < buffSize - 1; i++) {
       sendresbuf[i] = (rand() % ('z' - 'a') + 'a' + rank) % ('z' - 'a');
    }
    sendresbuf[buffSize - 1] = '\0';
  }

  double time = MPI_Wtime();
  MPI_Bcast(sendresbuf, buffSize, MPI_CHAR, root, MPI_COMM_WORLD);
  time = MPI_Wtime() - time;

  if (rank > 0) {
    printf("Process %d of %d on %s received message (%ld) with time \t= %.6lf\n", rank, commsize, procname, buffSize, time);
  }
  free(sendresbuf);
  //MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
  //MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE););
  MPI_Finalize();

  return 0;
}
