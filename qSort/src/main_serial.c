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

void quicksort_tasks(int *v, int low, int high) 
{
  int i, j;
  partition(v, &i, &j, low, high);
  if (low < j)
    quicksort_tasks(v, low, j);
  if(i < high)
    quicksort_tasks(v, i, high);
}

int main(int argc, char **argv)
{
  int size = 1000000;
  if (argc > 1)
    size = atoi(argv[1]);
  int *array = malloc(sizeof(int) * size);;
  for (int i = 0; i < size; ++i) {
    array[i] = rand() % size;
  }
  double time = -omp_get_wtime();
  quicksort_tasks(array, 0, size - 1);
  time += omp_get_wtime();
  printf("Quicksort serial\nSize array: %d\nTime: %lf\n",size, time);
  free(array);
  return 0;
}
