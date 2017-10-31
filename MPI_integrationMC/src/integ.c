#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <mpi.h>


const double PI = 3.14159265358979323846;

double getrand(unsigned int *seed)
{
  return (double)rand_r(seed) / RAND_MAX;
}

double func(double x, double y)
{
  return pow(exp(x + y), 2);
//  return 3 * pow(y, 2) * pow(sin(x), 2);
}

int main(int argc, char **argv)
{
  int in = 0;
  double s = 0;
  int n = atoi(argv[1]);
 // printf("Numerical integration by Monte Carlo method: n = %d\n", n);

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

  in = 0;
  s = 0;
  int seed = rank;
  if (rank == root) {
    printf("Numerical Integration by Monte Carlo method : n = %d\n", n); 
    time = MPI_Wtime();
  }

  double s_loc = 0;
  int in_loc = 0;

  for (int i = 0; i < n - commsize; i+=commsize) {
    double x = getrand(&seed) * PI; /* x in [0, 1] */
    double y = getrand(&seed);
    /* y in [0, 1 - x)] */
    if (y <= 1 - x) {
      in_loc++;
      s_loc += func(x, y);
    }
  }

  MPI_Reduce(&s_loc, &s, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
  MPI_Reduce(&in_loc, &in, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
  
  if (rank == root) {   
    time = MPI_Wtime() - time;
    double v = PI * in / n;
    double res = v * s / in;
    printf("Result: %.12f, n %d\ntime = %.6lf\n", res, n, time);
  }

  MPI_Finalize();
  return 0;
}
