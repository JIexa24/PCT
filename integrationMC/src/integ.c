#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <omp.h>


const double PI = 3.14159265358979323846;

double getrand(unsigned int *seed)
{
  return (double)rand_r(seed) / RAND_MAX;
}

double func(double x, double y)
{
  return 3 * pow(y, 2) * pow(sin(x), 2);
}

int main(int argc, char **argv)
{
  int thr = 2;
  const int n = 10000000;
  printf("Numerical integration by Monte Carlo method: n = %d\n", n);
  int in = 0;
  double s = 0;
  while (thr < 9) {
    in = 0;
    s = 0;
    double t = omp_get_wtime();
    #pragma omp parallel num_threads(thr)
    {
      double s_loc = 0;
      int in_loc = 0;
      unsigned int seed = omp_get_thread_num();
      #pragma omp for nowait
      for (int i = 0; i < n; i++) {
        double x = getrand(&seed) * PI; /* x in [0, pi] */
        double y = getrand(&seed);
        /* y in [0, sin(x)] */
        if (y <= sin(x)) {
        in_loc++;
        s_loc += func(x, y);
      }
    }
    #pragma omp atomic
      s += s_loc;
    #pragma omp atomic
      in += in_loc;
    }
    
    t = omp_get_wtime() - t;
    double v = PI * in / n;
    double res = v * s / in;
    printf("Result: %.12f, n %d\ntime = %.6lf\n", res, n, t);
    thr+=2;
  }
  return 0;
}
