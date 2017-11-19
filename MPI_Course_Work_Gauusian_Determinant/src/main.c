#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>


int get_chunk(int total, int commsize, int rank)
{
  int n = total;
  int q = n / commsize;
  if (n % commsize)
    q++;
  int r = commsize * q - n;
  /* Compute chunk size for the process */
  int chunk = q;
  if (rank >= commsize - r)
    chunk = q - 1;

  return chunk;
}

int main(int argc, char *argv[])
{
  int n = argc > 1 ? atoi(argv[1]) : 3000;
  int rank, commsize;
  int root = argc > 2 ? atoi(argv[2]) : 0;
  MPI_Init(&argc, &argv);
 
  double t = MPI_Wtime();

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  int nrows = get_chunk(n, commsize, rank);
  int *rows = malloc(sizeof(*rows) * nrows); // Номера локальных строк

  // Матрица дополнена столбцом для вектора b
  double determinant = 1;
  double *a = malloc(sizeof(*a) * nrows * (n + 1));
  double *tmp = malloc(sizeof(*tmp) * (n + 1));

  // Инициализация как в последовательной версии
  for (int i = 0; i < nrows; i++) {
    rows[i] = rank + commsize * i;
    srand(rows[i] * (n + 1));
    for (int j = 0; j < n; j++)
      a[i * (n + 1) + j] = rand() % 2 + 1;
    // b[i]
    a[i * (n + 1) + n] = rand() % 2 + 1;
}

#if 0
  MPI_Recv(NULL, 0, MPI_INT, (rank > 0) ? rank - 1 : MPI_PROC_NULL, 0, MPI_COMM_WORLD,
  MPI_STATUS_IGNORE); // Вывод в порядке: proc 0, 1, 2, … P-1.
  printf("Proc %d: ", rank);
  for (int i = 0; i < nrows; i++)
  printf("%d ", rows[i]);
  printf("\n");
  MPI_Ssend(NULL, 0, MPI_INT, rank != commsize - 1 ? rank + 1 : MPI_PROC_NULL, 0,
  MPI_COMM_WORLD);
#endif

// Прямой ход
  int row = 0;
  for (int i = 0; i < n - 1; i++) {
  // Исключаем x_i
    if (i == rows[row]) {
    // Рассылаем строку i, находящуюся в памяти текущего процесса
    MPI_Bcast(&a[row * (n + 1)], n + 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
    for (int j = 0; j <= n; j++)
      tmp[j] = a[row * (n + 1) + j];
    row++;
    } else {
      MPI_Bcast(tmp, n + 1, MPI_DOUBLE, i % commsize, MPI_COMM_WORLD);
    }
  // Вычитаем принятую строку из уравнений, хранящихся в текущем процессе
    for (int j = row; j < nrows; j++) {
      double scaling = a[j * (n + 1) + i] / tmp[i];
      for (int k = i; k < n + 1; k++)
        a[j * (n + 1) + k] -= scaling * tmp[k];
    }
  }

 
  if(rank == root) { 
    double locdet = 1;
    int row, j;
    for (j = rank, row = 0; row < nrows; j+=commsize, ++row) {
        determinant = determinant * a[row*(n+1) + j];
    }

    for (int i = 0; i < commsize; i++){
      if(i == root) continue;
      MPI_Recv(&locdet, 1,  MPI_DOUBLE, i,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      determinant = determinant * locdet;
    }

  }else{
    double locdet = 1;
    int row, j;
    for (j = rank, row = 0; row < nrows; j+=commsize, ++row) {
        locdet = locdet * a[row*(n+1) + j];

    }

    MPI_Send(&locdet, 1,  MPI_DOUBLE, root,0, MPI_COMM_WORLD);
  }
  t = MPI_Wtime() - t;

  free(tmp);
  free(rows);
  free(a);


if (rank == root) {
printf("Gaussian Determimant(MPI) = %lf: n %d, procs %d, time (sec) %.6f\n",
determinant , n, commsize, t);
}



MPI_Finalize();
return 0;
}
