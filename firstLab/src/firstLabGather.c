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
  if (rank == root) recvbuf = malloc(sizeof(char) * buffSize * (commsize - 1));
  else sendbuf = malloc(sizeof(char) * buffSize);
  MPI_Request req[commsize - 1];
  MPI_Request req1;
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
//      MPI_Recv(&(recvbuf[(i > root ? i - 1 : i) * buffSize]), buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Irecv(&(recvbuf[(i > root ? i - 1 : i) * buffSize]), buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, &(req[(i > root ? i - 1 : i)]));
    }
    MPI_Waitall(commsize - 1, req, MPI_STATUS_IGNORE);
  } else {
    MPI_Isend(sendbuf, buffSize, MPI_CHAR, root, 0, MPI_COMM_WORLD, &req1);
    MPI_Wait(&req1, MPI_STATUS_IGNORE);
  }

//  MPI_Gather(sendbuf, buffSize, MPI_CHAR, recvbuf, buffSize, MPI_CHAR, root, MPI_COMM_WORLD);
  time = MPI_Wtime() - time;
  int flag = 0;
  for (i = 0; i < buffSize; i++) {
     if (sendbuf[i] != recvbuf[i]) {
       flag = 1;
       break;
     }
  }
  if (rank > 0) {
    printf("Process %d of %d on %s(%d) received message (%ld) with time \t= %.6lf\n",rank,commsize,procname, flag,buffSize, time);
  }
//  MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
//  MPI_Recv(&recvbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  if (rank == root) free(recvbuf);
  else free(sendbuf);

  MPI_Finalize();
  return 0;
}
