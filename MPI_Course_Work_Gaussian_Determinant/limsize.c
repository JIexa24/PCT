#include <stdio.h>

int main()
{
  long n = 10000;
  long p = 8;
  double limitmemorynode = 7.0 * (1 << 30) / p;//7765180kB memFree (cat /proc/meminfo on cn1)
  double a1;
  while (1)
  {
    a1 = 8*n*n/p;
    double a2 = 8*(n+5)*(n+5)/p;
    if (a1 < limitmemorynode && a2 > limitmemorynode) break;
    n+=5;
  }
  printf("%ld %ld %lf\n", n, p, limitmemorynode / 1024);
  return 0;
}
