#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

struct thread_data {
  double sum;
  uint8_t padding[64 - sizeof(double)];
};

double func(double x)
{
  return exp(-x * x);
}

int main(int argc, char **argv)
{
  int thr = 2;
  while(thr < 9) {

  const double a = -4.0;
  const double b = 4.0;
  const int n = 10000000;
  printf("Numerical integration: [%f, %f], n = %d\n", a, b, n);
  double h = (b - a) / n;
  double s = 0.0;
//  struct thread_data sumloc[omp_get_max_threads()];
  double sumloc[thr];
  double t = omp_get_wtime();

  #pragma omp parallel num_threads(thr)
  {
    int tid = omp_get_thread_num();
    int points_per_thread = n / thr;
    int lo = tid * points_per_thread;
    int hi = (tid == thr - 1) ? n - 1 : lo + points_per_thread;
    sumloc[tid] = 0.0;

    for (int i = lo; i <= hi; i++)
      sumloc[tid] += func(a + h * (i + 0.5));
/*
    sumloc[tid].sum = 0.0;
    for (int i = lo; i <= hi; i++)
      sumloc[tid].sum += func(a + h * (i + 0.5));
*/
    #pragma omp atomic
      s += sumloc[tid];
//      s += sumloc[tid].sum;
}
  t = omp_get_wtime() - t;
  s *= h;
  printf("Result Pi: %.12f\n", s * s);
  printf("Elapsed time (sec.): %.12f\n", t);
  thr += 2;
  }
  return 0;
}
