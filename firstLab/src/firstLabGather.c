#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
  long buffSize = atol(argv[1]);
  int rank,commsize;
  int root = 0;
  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);

  char *resbuf;
  char *sendbuf;
  int i = 0;
  if (rank > 0) {
    sendbuf = malloc(sizeof(char) * buffSize);
    for (i = 0; i < buffSize - 1; i++) {
       sendbuf[i] = (rand() % ('z' - 'a') + 'a' + rank) % ('z' - 'a');
    }
    sendbuf[buffSize - 1] = '\0';
  } else if (rank == 0){
    resbuf = malloc(sizeof(char) * buffSize);
  }

  double time = MPI_Wtime();
  MPI_Gather(sendbuf, buffSize, MPI_CHAR, resbuf, buffSize, MPI_CHAR, root, MPI_COMM_WORLD);
  time = MPI_Wtime() - time;

  if (rank > 0) {
    printf("Process %d of %d on %s received message (%ld) with time \t= %.6lf\n", rank, commsize, procname, buffSize, time);
  }
//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&resbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  if (rank == 0) {
    free(resbuf);
  } else if (rank > 0) {
    free(sendbuf);
  }
  MPI_Finalize();

  return 0;
}
