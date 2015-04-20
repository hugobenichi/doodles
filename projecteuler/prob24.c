#include <stdio.h>
#include <stdlib.h>

int permutation(long long y) // permutations of {0,1,2,3,4,5,6,7,8,9}
{
  int i = 0;
  int mask = 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512;
  for (; i < 10; i++)
  {
    int bit = y % 10;
    mask -= (1 << bit);
    y /= 10;
  }

  return mask;
}

int main(void)
{

  int i = 362880; //1;
  int lim = 1000000;

  long long x = 1023456789; //123456789;
  long long y = x;

  while(lim - i)
  {
    y += 1;
    if ( !permutation(y) )
      i += 1;
  }

  printf("%.10lli\n",y);

  exit(0);
}
