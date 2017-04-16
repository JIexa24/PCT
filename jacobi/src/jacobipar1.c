#include <inttypes.h>
#include <math.h>
#include <float.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define EPS 0.001
#define PI 3.14159265358979323846
#define IND(i, j) ((i) * nx + (j))

double wtime()
{
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main(int argc, char *argv[])
{
  int thr = 0;
  double ttotal = -omp_get_wtime();
  int rows = (argc > 1) ? atoi(argv[1]) : 100;
  int cols = (argc > 2) ? atoi(argv[2]) : 100;
  const char *filename = (argc > 3) ? argv[3] : NULL;
  if (cols < 1 || rows < 1) {
    fprintf(stderr, "Invalid size of grid: rows %d, cols %d\n", rows, cols);
    exit(EXIT_FAILURE);// Allocate memory for grids [0..ny - 1][0..nx - 1]
  }

  int ny = rows;    
  int nx = cols;

  double talloc = -wtime();
  //double *local_grid = cmalloc(ny * nx, sizeof(*local_grid));
  //double *local_newgrid = cmalloc(ny * nx, sizeof(*local_newgrid));
  double *local_grid = malloc(ny * nx * sizeof(*local_grid));
  double *local_newgrid = malloc(ny * nx * sizeof(*local_newgrid));
  talloc += wtime();
  thr = 2;
  while(thr < 9) {
  omp_set_num_threads(thr);
  double tinit = -omp_get_wtime();
  // Fill boundary points:
  //- left and right borders are zero filled
  //- top border: u(x, 0) = sin(pi * x)
  //- bottom border: u(x, 1) = sin(pi * x) * exp(-pi)
  double dx = 1.0 / (nx - 1.0);
  // Initialize top border: u(x, 0) = sin(pi * x)

  #pragma omp parallel for
  for (int j = 0; j < nx; j++) {
    int ind = IND(0, j);
    local_newgrid[ind] = local_grid[ind] = sin(PI * dx * j);
  }
  // Initialize bottom border: u(x, 1) = sin(pi * x) * exp(-pi)
  #pragma omp parallel for
  for (int j = 0; j < nx; j++) {
    int ind = IND(ny - 1, j);
    local_newgrid[ind] = local_grid[ind] = sin(PI * dx * j) * exp(-PI);
  }
  #pragma omp parallel  for
  for (int i = 1; i < ny - 1; i++) {
    for (int j = 1; j < nx - 1; j++) {
      local_newgrid[IND(i, j)] = 0.0;
      local_grid[IND(i, j)] = 0.0;
    }
  }

  tinit += omp_get_wtime();

  int niters = 0;
  for (;;) {
    niters++;
    #pragma omp parallel for 
    for (int i = 1; i < ny - 1; i++) {
      for (int j = 1; j < nx - 1; j++) {
        local_newgrid[IND(i, j)] = (local_grid[IND(i - 1, j)] + local_grid[IND(i + 1, j)] +
                              local_grid[IND(i, j - 1)] + local_grid[IND(i, j + 1)]) * 0.25;
      }
    }
    double maxdiff = -DBL_MAX;
    #pragma omp parallel for reduction(max:maxdiff)
    for (int i = 1; i < ny - 1; i++) {
      for (int j = 1; j < nx - 1; j++) {
      int ind = IND(i, j);
      maxdiff = fmax(maxdiff, fabs(local_grid[ind] - local_newgrid[ind]));
      }
    }
    double *p = local_grid; local_grid = local_newgrid; local_newgrid = p;
    if (maxdiff < EPS) break;
  }

  printf("# Heat 2D (serial): grid: rows %d, cols %d\n", rows, cols);
  printf("# niters %d, total time (sec.): %.6f\n", niters, ttotal+ omp_get_wtime());
  printf("# talloc: %.6f, tinit: %.6f, titers: %.6f\n", talloc, tinit, ttotal - talloc - tinit +  omp_get_wtime());
  // Save grid
  if (filename) {
     FILE *fout = fopen(filename, "w");
     if (!fout) {
       perror("Can't open file");
       exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ny; i++) {
      for (int j = 0; j < nx; j++)
        fprintf(fout, "%.4f ", local_grid[IND(i, j)]);
      fprintf(fout, "\n");
    }
    fclose(fout);
  }


  thr +=2;
  }
  return 0;
}
