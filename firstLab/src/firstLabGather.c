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
  char *recvbuf;
  char *sendbuf;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);

  MPI_Request req[commsize - 1];
  MPI_Status stat[commsize - 1];
  if (rank == root) recvbuf = malloc(sizeof(char) * buffSize * (commsize - 1));
  else sendbuf = malloc(sizeof(char) * buffSize);

  int i = 0;
  if (rank > 0) {
    for (i = 0; i < buffSize - 1; i++) {
       sendbuf[i] = (rand() % ('z' - 'a') + 'a' + rank) % ('z' - 'a');
    }
    sendbuf[buffSize - 1] = '\0';
  }

  double time = MPI_Wtime();
  if (rank == root) {
    for (i = 0; i < commsize; i++) {
      if (i == root) continue;
      MPI_Irecv(&(recvbuf[(i > root ? i - 1 : i) * buffSize]), buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, &(req[(i > root ? i - 1 : i)]));
      MPI_Waitall(commsize - 1, req, stat);
    }
  } else {
    MPI_Send(&sendbuf, buffSize, MPI_CHAR, root, 0, MPI_COMM_WORLD);
  }
//  MPI_Gather(sendbuf, buffSize, MPI_CHAR, recvbuf, buffSize, MPI_CHAR, root, MPI_COMM_WORLD);
  time = MPI_Wtime() - time;

  if (rank > 0) {
    printf("Process %d of %d on %s received message (%ld) with time \t= %.6lf\n", rank, commsize, procname, buffSize, time);
  }
//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&recvbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  if (rank == root) free(recvbuf);
  else free(sendbuf);

  MPI_Finalize();
  return 0;
}
