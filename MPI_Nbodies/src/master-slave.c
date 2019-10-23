#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>
//MPI_ANY_SOURCE
double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

struct particle
{
    double x, y, z;
};

const double G = 6.67e-11;

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

void calculate_forces(struct particle *p, struct particle *f, double *m, int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = sqrtf(powf(p[i].x - p[j].x, 2) + powf(p[i].y - p[j].y, 2) +
                               powf(p[i].z - p[j].z, 2));
            double mag = (G * m[i] * m[j]) / powf(dist, 2);
            struct particle dir = {
                    .x = p[j].x - p[i].x,
                    .y = p[j].y - p[i].y,
                    .z = p[j].z - p[i].z
            };
            f[i].x += mag * dir.x / dist;
            f[i].y += mag * dir.y / dist;
            f[i].z += mag * dir.z / dist;
            f[j].x -= mag * dir.x / dist;
            f[j].y -= mag * dir.y / dist;
            f[j].z -= mag * dir.z / dist;
        }
    }
}

void move_particles(struct particle *p, struct particle *f, struct particle *v, double *m, int n, double dt)
{
    for (int i = 0; i < n; i++) {
        struct particle dv = {
                .x = f[i].x / m[i] * dt,
                .y = f[i].y / m[i] * dt,
                .z = f[i].z / m[i] * dt,
        };
        struct particle dp = {
                .x = (v[i].x + dv.x / 2) * dt,
                .y = (v[i].y + dv.y / 2) * dt,
                .z = (v[i].z + dv.z / 2) * dt,
        };
        v[i].x += dv.x;
        v[i].y += dv.y;
        v[i].z += dv.z;
        p[i].x += dp.x;
        p[i].y += dp.y;
        p[i].z += dp.z;
        f[i].x = f[i].y = f[i].z = 0;
    }
}

void master()
{
  int i, j , iter = 0, chunk, recvrank;
  for (i = 0; i < commsize; i++) {
    for (j = i; j < commsize; j++) {
      if (iter < commsize) {
        MPI_Send(task to iter);
        iter++;
      } else {
        MPI_Recv(rank from MPI_ANY_SOURCE);
        MPI_Send(task to rank);
      }
    }
  }
}

void slave()
{
  while (1) {
    MPI_Recv(task);
    if (task != (0,0)) {

    } else break;
  }
}

int main(int argc, char *argv[])
{
  double ttotal, tinit = 0, tforces = 0, tmove = 0;
  ttotal = wtime();
  int n = (argc > 1) ? atoi(argv[1]) : 10;
  char *filename = (argc > 2) ? argv[2] : NULL;
  int rank, commsize, chunk;
  tinit = -wtime();
    
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    
  struct particle *p = malloc(sizeof(*p) * n); // Положение частицы
  struct particle *f = malloc(sizeof(*f) * n); // Сила, действующая на каждую частицу
  struct particle *v = malloc(sizeof(*v) * n); // Скорость частицы
  double *m = malloc(sizeof(*m) * n); // Масса частицы

  if (rank == root) {
    for (int i = 0; i < n; i++)
    {
      p[i].x = rand() / (double)RAND_MAX - 0.5;
      p[i].y = rand() / (double)RAND_MAX - 0.5;
      p[i].z = rand() / (double)RAND_MAX - 0.5;
      v[i].x = rand() / (double)RAND_MAX - 0.5;
      v[i].y = rand() / (double)RAND_MAX - 0.5;
      v[i].z = rand() / (double)RAND_MAX - 0.5;
      m[i] = rand() / (double)RAND_MAX * 10 + 0.01;
      f[i].x = f[i].y = f[i].z = 0;
    }
  }
  MPI_Bcast(p);
  MPI_Bcast(v);
  MPI_Bcast(m);
  MPI_Bcast(f);
  tinit += wtime();

  double dt = 1e-5;
  
  if (rank == root) {
  	master();
  } else {
  	slave();
  }
    
  ttotal = wtime() - ttotal;
  printf("# NBody (n=%d)\n", n);
  printf("# Elapsed time (sec): ttotal %.6f, tinit %.6f, tforces %.6f, tmove %.6f\n",
          ttotal, tinit, tforces, tmove);
  if (filename)
  {
    FILE *fout = fopen(filename, "w");
    if (!fout) {
      fprintf(stderr, "Can't save file\n");
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
      fprintf(fout, "%15f %15f %15f\n", p[i].x, p[i].y, p[i].z);
    }
    fclose(fout);
  }
  
  MPI_Finalize();
  
  free(m);
  free(v);
  free(f);
  free(p);
  return 0;
}
