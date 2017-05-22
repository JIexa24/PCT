#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

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
omp_lock_t *locks;

void calculate_forces(struct particle *p, struct particle *f, double *m, int n)
{
    #pragma omp for schedule(dynamic, 4) nowait
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            float dist = sqrtf(powf(p[i].x - p[j].x, 2) +
                               powf(p[i].y - p[j].y, 2) +
                               powf(p[i].z - p[j].z, 2));
            float mag = (G * m[i] * m[j]) / powf(dist, 2);
            struct particle dir = {
                    .x = p[j].x - p[i].x,
                    .y = p[j].y - p[i].y,
                    .z = p[j].z - p[i].z
            };
            f[i].x += mag * dir.x / dist;
            f[i].y += mag * dir.y / dist;
            f[i].z += mag * dir.z / dist;
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

int main(int argc, char *argv[])
{
    omp_set_num_threads(atoi(argv[1]));
    double ttotal, tinit = 0, tforces = 0, tmove = 0;
    ttotal = wtime();
    int n = (argc > 1) ? atoi(argv[1]) : 10;
    char *filename = (argc > 2) ? argv[2] : NULL;
    tinit = -wtime();
    struct particle *p = malloc(sizeof(*p) * n); // Положение частицы
    struct particle *f = malloc(sizeof(*f) * n); // Сила, действующая на каждую частицу
    struct particle *v = malloc(sizeof(*v) * n); // Скорость частицы
    double *m = malloc(sizeof(*m) * n); // Масса частицы
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

    locks = malloc(sizeof(omp_lock_t) * n);
    for (int i = 0; i < n; i++)
        omp_init_lock(&locks[i]);
    double dt = 1e-5;
    tinit += wtime();
    #pragma omp parallel
    {
        for (double t = 0; t <= 1; t += dt) {
            calculate_forces(p, f, m, n);
            #pragma omp barrier
            move_particles(p, f, v, m, n, dt);
            #pragma omp barrier
        }
    }

    free(locks);
    ttotal = wtime() - ttotal;
    printf("# NBody (n=%d)\n", n);
    printf("# Elapsed time (sec): ttotal %.6f, tinit %.6f\n", ttotal, tinit);
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
    free(m);
    free(v);
    free(f);
    free(p);
}
