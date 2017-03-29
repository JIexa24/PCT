#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <omp.h>

double func(double x)
{
  return exp(-x * x);
}

int main(int argc, char **argv)
{

  const double a = -4.0;
  const double b = 4.0;
  const int n = 10000000;
  printf("Numerical integration: [%f, %f], n = %d\n", a, b, n);
  double h = (b - a) / n;
  double s = 0.0;

  double t = omp_get_wtime();
  for (int i = 0; i < n; i++)
    s += func(a + h * (i + 0.5));

  s *= h;
  t = omp_get_wtime() - t;
  printf("Result Pi: %.12f\n", s * s);

  printf("Serial time (sec.): %.12f\n", t);
  return 0;
}
