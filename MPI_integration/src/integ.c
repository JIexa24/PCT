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
  double a = atof(argv[1]);
  double b = atof(argv[2]);
  int n = atoi(argv[3]);
  double h = (b - a) / n;
  double sum = 0.0;
  double sumloc = 0.0;
  double tsumloc = 0.0;
  double eps = 0.01;
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

  do {
    for (i = rank; i < n - commsize; i += commsize)
      tsumloc += func(a + h * (i + 0.5));

    sumloc = tsumloc;  
    h = h / 2;
    
  } while (fabs(tsumloc - sumloc) > eps);  
 
  MPI_Reduce(&sumloc, &sum, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

  if (rank == root) {
    sum = sum * h;
    time = MPI_Wtime() - time;
    printf("Process %d of %d on %s. S = %lf with time \t= %.6lf\n",rank,commsize, procname, sum * sum ,time);
  }
  MPI_Finalize();
  return 0;
}
