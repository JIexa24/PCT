#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

double func(double x)
{
  return exp(-x * x);
}

int main(int argc,char **argv)
{
  assert(!(argc < 5));
  double a = atof(argv[1]);
  double b = atof(argv[2]);
  long int n = atol(argv[3]) / 2;
  double h = (b - a) / n;
  double sum = 0.0;
  double sumloc = 0.0;
  double tsumloc = 0.0;
  double eps = atof(argv[4]);
  int root = 0;
  int rank,commsize;
  long int i = 0;
  int len;
  char procname[MPI_MAX_PROCESSOR_NAME];
  double time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(procname, &len);

  if (rank == root)
    time = MPI_Wtime();

  do {
    tsumloc = sumloc;
    sumloc = 0.0;
    n = n * 2;
    h = (b - a) / n;
    
    for (i = rank; i < n - commsize; i += commsize)
      sumloc += func(a + h * (i + 0.5));
    sumloc *= h;
  } while (fabs(tsumloc - sumloc) * h > eps);  
 
  MPI_Reduce(&sumloc, &sum, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

  if (rank == root) {
  //  sum = sum * h;
    time = MPI_Wtime() - time;
    printf("Process %d of %d on %s. S = %lf with time \t= %.6lf (%d|%lf)\n",rank,commsize, procname, sum * sum ,time, n, h);
  }
  MPI_Finalize();
  return 0;
}
