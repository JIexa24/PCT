#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <omp.h>

const double PI = 3.14159265358979323846;
const int n = 10000000;

double getrand()
{
  return (double)rand() / RAND_MAX; 
}

double func(double x, double y)
{
  return 3 * pow(y, 2) * pow(sin(x), 2);
}

int main(int argc, char **argv)
{
  int in = 0;
  double s = 0;
  double t = omp_get_wtime();
  for (int i = 0; i < n; i++) {
    double x = getrand() * PI;
    double y = getrand();
    if (y <= sin(x)) {
      in++;
      s += func(x, y);
    } 
  }
  t = omp_get_wtime() - t;
  double v = PI * in / n;
  double res = v * s / in;
  /* x in [0, pi] */
  /* y in [0, sin(x)] */
  printf("Result: %.12f, n %d\ntime = %.6lf\n", res, n, t);
  return 0;
}
