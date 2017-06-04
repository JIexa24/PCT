#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void partition(int *v, int *i, int *j, int low, int high) {
  *i = low;
  *j = high;
  int pivot = v[(low + high) / 2];
  do {
    while (v[*i] < pivot) (*i)++;
    while (v[*j] > pivot) (*j)--;
      if (*i <= *j) {
        int tmp = v[*i];
        v[*i] = v[*j];
        v[*j] = tmp;
        (*i)++;
        (*j)--;
      }
  } while (*i <= *j);
}

void quicksort_tasks(int *v, int low, int high, int threshold) {
  int i, j;
  partition(v, &i, &j, low, high);
  if (high - low < threshold || (j - low < threshold || high - i < threshold)) {
    if (low < j)
      quicksort_tasks(v, low, j, threshold);
    if(i < high)
      quicksort_tasks(v, i, high, threshold);
  } else {
    #pragma omp task
    {
      quicksort_tasks(v, low, j, threshold);
    }
    quicksort_tasks(v, i, high, threshold);
  }
}

int main(int argc, char **argv) {

  int size = 1000000;
  if (argc > 1)
    size = atoi(argv[1]);
  int *array = malloc(sizeof(int) * size);;
  for (int i = 0; i < size; ++i) {
    array[i] = rand() % size;
  }
  int threshold = 1000;
  if (argc > 2)
    threshold = atoi(argv[2]);
  double time;
  int numthreads = 2;
  #pragma omp parallel num_threads(2)
  {
    #pragma omp single
    numthreads = omp_get_num_threads();
    #pragma omp single
    time = -omp_get_wtime();
    #pragma omp single
    quicksort_tasks(array, 0, size - 1, threshold);
    #pragma omp single
    time += omp_get_wtime();
  }
  printf("Quicksort parallel threads =  (%d)\nSize array: %d\nThreshold: %d\nTime: %lf\n",numthreads,size,threshold, time);
  free(array);
  return 0;
}
