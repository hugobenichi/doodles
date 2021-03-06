#include <stdio.h>
#include <stdlib.h>

void sleeve(int* table, int len)
{
  
  table[0] = 0;
  table[1] = 0;
  int i = 2;
  while(len-i)
    table[i++] = 1;
      
  i = 2;
  while(len-i)
  {
    if(table[i])
    {
      int k = 2*i;
      while(k < len)
      {
        table[k] = 0;
        k += i;
      }
    }
    i++;
  }
  
}

int is_pandigital(int val)
{
  int flags = 0;
  int len = 0;
  int rez = 1;
  
  while(rez && val > 0)
  {
    int bit = 1 << ( (val % 10) - 1 ); // 1 at bit index 0
    if( flags & bit )
      rez = 0;
    flags += bit;
    val /= 10;
    len++;
  }
  
  while(rez && len)
  {
    if ( !(flags & 0x0001) )
      rez = 0;
    flags >>= 1;
    len--;
  }
  
  return rez;
}

int main(void)
{

  int lim = 100000000; //9876543; //21;
  
  int *primes = malloc( lim * sizeof(int) );
  sleeve(primes, lim);

  int i = 1000;
  int best = 0;
  while( lim -i )
  {
    if ( primes[i] && is_pandigital(i) )
      best = i;
    i++;
  }
  
  printf("%i\n",best);
  
  free(primes);

  exit(0);
}
