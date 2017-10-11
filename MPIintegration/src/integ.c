#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double func(double x)
{
  return exp(-x * x);
}

int main(int argc,char **argv)
{
  double a = atol(argv[1]);
  double b = atol(argv[2]);
  int n = atoi(argv[3]);
  double h = (b - a) / n;
  double sum = 0.0;
  double sumloc = 0.0;
  int root = 0;
  int rank,commsize;
  int i = 0;
  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];
  double time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);

  if (rank == root)
    time = MPI_Wtime();

  while (1) {
    if (a +  h * commsize * i >= b) break;
    sumloc += func(a + h * (i++ + 0.5));
  }
  sumloc = sumloc * h;
  if (rank == root) {
    MPI_Reduce(&sumloc, &sum, 1, MPI_DOUBLE, 0, root, MPI_COMM_WORLD);
    time = MPI_Wtime() - time;
    printf("Process %d of %d on %s. S = %lf with time \t= %.6lf\n",rank,commsize, procname,sum,time);
  }
  MPI_Finalize();
  return 0;
}
