/*
 * counter.c:
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <inttypes.h>

typedef struct Count {
  int count;
  uint8_t padding[64 - sizeof(int)];
} counter;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main(int argc, char *argv[])
{
    omp_set_num_threads(2);
    int n = 100000000;
    int *v = malloc(sizeof(*v) * n);

    for (int i = 0; i < n; i++)
        v[i] = rand() % 30;

    counter c[8];
    for (int i = 0; i < 8; i++)
      c[i].count = 0;

    int count = 0;
    double t = wtime();
    #pragma omp parallel num_threads(2)
    {
      #pragma omp for
      for (int i = 0; i < n; i++) {
        if (v[i] == 3) {
          c[tid].count++;
        }
      }
    }
    for(int i = 0; i < 8; i++)
     count+=c[i].count;
    t = wtime() - t;
    printf("Counter (par, n = %d)\n", n);
    printf("Count = %d\n", count);
    printf("Time (sec): %.6f\n", t);

    free(v);
    return 0;
}
