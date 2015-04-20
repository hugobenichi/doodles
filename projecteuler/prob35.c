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

int main(void)
{

  int lim = 1000000;
  int* primes = malloc( sizeof(int) * lim);
  sleeve(primes, lim);
   
  int x = 2;
  int sum = 0;
  int ord = 1;
  
  while ( x < lim )
  {
    if( ord*10 < x) ord *= 10; 
    if(primes[x])
    {
      int y = x;
      int circ_prime = 1;
      do {
        y = ( y % ord ) * 10 + y / ord;
        if( !primes[y] )
        {
          circ_prime = 0;
          break; 
        }
      } while( y != x);
      if( circ_prime )
        sum++;
    }
    x++;
  }
  
  printf("%i\n", sum);

  free(primes);

  exit(0);  
}
