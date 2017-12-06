#include <stdio.h>

int main()
{
  long n = 10000;
  long nod = 18;
  long p = 8 * nod;
  double limitmemorynode = (1 << 30);
  double a1, a2;
  while (1)
  {
    a1 = 8*(n*n/p+ 2 * n);
    a2 = 8*((n+5)*(n+5)/p+ 2 * (n+5));
    if (a1 < limitmemorynode && a2 > limitmemorynode) break;
    n+=5;
  }
  
  printf("%ld %ld %lf < %lf < %lf\n", n , p,a1/1024, limitmemorynode / 1024,a2/1024);
  return 0;
}
