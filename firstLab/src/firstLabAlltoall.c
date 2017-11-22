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
  int blocksize = 4;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);
  MPI_Request *req = malloc(sizeof(MPI_Request) * 2 * (commsize - 2));
  recvbuf = malloc(sizeof(char) * buffSize * (commsize));
  sendbuf = malloc(sizeof(char) * buffSize);
  MPI_Request *reqs = (MPI_Request*)malloc(sizeof(MPI_Request) * blocksize * 2);

  MPI_Request *reqs1 = malloc(sizeof(MPI_Request) * blocksize * 2);

  int i = 0;
  for (i = 0; i < buffSize - 1; i++) {
     sendbuf[i] = (rand() % ('z' - 'a') + 'a' + rank) % ('z' - 'a');
  }
  sendbuf[buffSize - 1] = '\0';
  int k;
  int locsize = commsize > blocksize ? blocksize : commsize;
  int t = commsize;
  double time = MPI_Wtime();
  
  for (k = 0; k < commsize; k += blocksize){
    for (i = 0; i < locsize; i++)
    {   
       MPI_Isend(sendbuf, buffSize, MPI_CHAR, i + k, 0, MPI_COMM_WORLD, &(reqs[i]));
    }

    for (i = 0; i < locsize; i++)
    {    
      MPI_Irecv(&(recvbuf[(i+k) * buffSize]), buffSize, MPI_CHAR, i + k, 0, MPI_COMM_WORLD, &(reqs[locsize + i]));
    }
    MPI_Waitall(locsize * 2, reqs, MPI_STATUS_IGNORE);
    t -= blocksize;
    locsize = t > blocksize ? blocksize : t;
  }

/*
  for (i = 0; i < commsize; i++) {
    if (i == rank) continue;
      MPI_Isend(sendbuf, buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, &(req[(i > rank ? i - 1 : i)]));
      MPI_Irecv(&(recvbuf[(i > rank ? i - 1 : i) * buffSize]), buffSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, &(req[(i > rank ? (i + commsize - 2) : (i + commsize - 1))]));
  }
  MPI_Waitall(2*(commsize - 2), req, MPI_STATUS_IGNORE);
*/

/*  MPI_Alltoall(sendbuf, buffSize, MPI_CHAR,
               recvbuf, buffSize, MPI_CHAR, MPI_COMM_WORLD);  */
  time = MPI_Wtime() - time;
  int flag = 0;
  for (i = 0; i < buffSize - 1; i++) {
     if (sendbuf[i] != recvbuf[i]) {
       flag = 1;
     }
  }

  printf("Process %d of %d on %s(%d) received message (%ld) with time \t= %.6lf\n", rank, commsize, procname, flag, buffSize, time);

  //MPI_Send(&sendbuf, buffSize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
  //MPI_Recv(&recvbuf, buffSize, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  free(sendbuf);
  free(recvbuf);
  free(req);  free(reqs);  free(reqs1);
  MPI_Finalize();

  return 0;
}
