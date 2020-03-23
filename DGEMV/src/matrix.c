#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>
int thr;
double speedup = 0;
double ser = 0;

//int m = 15000, n = 15000;

enum sz {
m = 25000, n = 25000
};

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void matrix_vector_product(double *a, double *b, double *c, enum sz m, enum sz n)
{
  for (int i = 0; i < m; i++) {
    c[i] = 0.0;
    for (int j = 0; j < n; j++)
      c[i] += a[i * n + j] * b[j];
  }
}

void run_serial()
{
  double *a, *b, *c;
  a = malloc(sizeof(*a) * m * n);
  b = malloc(sizeof(*b) * n);
  c = malloc(sizeof(*c) * m);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++)
      a[i * n + j] = i + j;
  }

  for (int j = 0; j < n; j++)
    b[j] = j;

  double t = wtime();

  matrix_vector_product(a, b, c, m, n);

  t = wtime() - t;
  speedup = t;
  printf("Elapsed time (serial): %.6f sec.\n", t);

  free(a);
  free(b);
  free(c);
}
/* matrix_vector_product_omp: Compute matrix-vector product c[m] = a[m][n] * b[n] */
void matrix_vector_product_omp(double *a, double *b, double *c, enum sz m, enum sz n)
{
  #pragma omp parallel num_threads(thr)
  {
    int nthreads = omp_get_num_threads();
 //   printf("!!%d!!",thr);
    int threadid = omp_get_thread_num();
    int items_per_thread = m / nthreads;
    int lb = threadid * items_per_thread;
    int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);

    for (int i = lb; i <= ub; i++) {
      c[i] = 0.0;
      for (int j = 0; j < n; j++)
        c[i] += a[i * n + j] * b[j];
      }
    }
}
void matrix_vector_product_omp_v2(double *a, double *b, double *c, enum sz m, enum sz n) {
  #pragma omp parallel for num_threads(thr)
  for (int i = 0; i < m; i++) {
    c[i] = 0.0;
    for (int j = 0; j < n; j++)
      c[i] += a[i * n + j] * b[j];
    }
  }
}

void run_parallel()
{
  double *a, *b, *c;
  // Allocate memory for 2-d array
  a = malloc(sizeof(*a) * m * n);
  b = malloc(sizeof(*b) * n);
  c = malloc(sizeof(*c) * m);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++)
      a[i * n + j] = i + j;
  }

  for (int j = 0; j < n; j++)
    b[j] = j;

  double t = wtime();
  matrix_vector_product_omp(a, b, c, m, n);
  t = wtime() - t;
  speedup = ser / t;
  printf("Elapsed time (parallel): %.6f sec.\n", t);
  free(a);
  free(b);
  free(c);
}

int main(int argc, char **argv)
{
  thr = atoi(argv[1]);{
  ser = atof(argv[2]);
//  printf("Matrix-vector product (c[m] = a[m, n] * b[n]; m = %d, n = %d)\n", m, n);
//  printf("Memory used: %" PRIu64"  MiB\n", ((m * n + m + n) * sizeof(double)) >> 20);
  for(thr; thr < 9;thr += 2) {
    run_serial();
    run_parallel();
    printf("\nspeedup %d = %.6lf\n",thr, speedup);
    speedup = 0;
  }
  return 0;
}
