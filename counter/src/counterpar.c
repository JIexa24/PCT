/*
 * counter.c: 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main(int argc, char *argv[])
{
    omp_set_num_threads(8);
    int n = 100000000;
    int *v = malloc(sizeof(*v) * n);

    for (int i = 0; i < n; i++)
        v[i] = rand() % 30;
    
    int c[8] = {0,0,0,0,0,0,0,0};
    int count = 0;
    double t = wtime();
    #pragma omp parallel 
    {
    int tid = omp_get_thread_num();
    #pragma omp for
    for (int i = 0; i < n; i++) { 
        if (v[i] == 3) {
            c[tid]++;
        }
    }
    }
    for(int i = 0; i < 8; i++)
     count+=c[i];
    t = wtime() - t;
    printf("Counter (par, n = %d)\n", n);    
    printf("Count = %d\n", count);    
    printf("Time (sec): %.6f\n", t);
    
    free(v);
    return 0;
}
