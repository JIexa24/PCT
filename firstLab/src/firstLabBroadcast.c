#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
  long buffSize = atol(argv[1]);
  int rank,commsize;
  int root = 0;
  int len;
  char *recvbuf;
  char *sendbuf;
  char procname[MPI_MAX_PROCESSOR_NAME];

  MPI_Comm comm;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);
  MPI_Request req[commsize - 1];
  MPI_Request req1;

  int i = 0;
  if (rank == root) sendbuf = malloc(sizeof(char) * buffSize);
  else recvbuf = malloc(sizeof(char) * buffSize);
  if (rank == 0) {
    for (i = 0; i < buffSize - 1; i++) {
       sendbuf[i] = (rand() % ('z' - 'a') + 'a' + rank) % ('z' - 'a');
    }
    sendbuf[buffSize - 1] = '\0';
  }

  double time = MPI_Wtime();
  if (rank == root) {
    for (i = 0; i < commsize; i++) {
      if (i == root) continue;
      MPI_Isend(&sendbuf, buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, &(req[(i > root ? i - 1 : i)]));
    }
    MPI_Waitall(commsize - 1, req, MPI_STATUS_IGNORE);
  } else {
    MPI_Irecv(recvbuf, buffSize, MPI_CHAR, root, 0, MPI_COMM_WORLD, &req1);
    MPI_Waitall(1, &req1, MPI_STATUS_IGNORE);
  }
  //MPI_Bcast(sendrecvbuf, buffSize, MPI_CHAR, root, MPI_COMM_WORLD);
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
  free(sendbuf);
  free(recvbuf);
  //MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
  //MPI_Recv(&recvbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE););
  MPI_Finalize();

  return 0;
}
