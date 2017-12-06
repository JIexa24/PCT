#include <stdio.h>

int main()
{
  long n = 10000; //start n
  long p = 8; // one node
  double limitmemorynode = 7.0 * (1 << 30) / p;//7765180kB memFree (cat /proc/meminfo on cn1)
  double a1,a2;
  while (1)
  {
    a1 = 8*n*n/p;
     a2 = 8*(n+5)*(n+5)/p;
    if (a1 < limitmemorynode && a2 > limitmemorynode) break;
    n+=5;
  }
  printf("%ld %ld %lf<%lf<%lf\n", n, p, a1 / 1024, limitmemorynode / 1024, a2 / 1024);
  return 0;
}
