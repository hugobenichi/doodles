#include <stdio.h>
#include <stdlib.h>

int is_prod_pandigital(int a, int b)
{
  int flags = 0;
  int len = 0;
  int rez = 1;
  int c = a*b;
  while(rez && a > 0)
  {
    int bit = 1 << ((a % 10) - 1);
    if( flags & bit )
      rez = 0;
    flags += bit;
    a /= 10;
    len++;
  }
  while(rez && b > 0)
  {
    int bit = 1 << ((b % 10) - 1);
    if( flags & bit )
      rez = 0;
    flags += bit;
    b /= 10;
    len++;
  }
  while(rez && c > 0)
  {
    int bit = 1 << ((c % 10) - 1);
    if( flags & bit )
      rez = 0;
    flags += bit;
    c /= 10;
    len++;
  }    
  rez *= len;
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
  
  
  int i, j, lim = 10000, sum = 0;
  int* found = malloc( sizeof(int) * 10000);
  
  for(i = 0; i < 10000; i++)
    found[i] = 0;
  
  /* this twin loop could be made much faster, 
   * by killing some terms like 1000 * 1000
   * but too lazy
   */
  i = 1;
  while ( lim - i)
  {
    j = i+1;
    while ( lim - j)
    {
      int rez = 0;
      if ( 9 == is_prod_pandigital(i,j))    // can calculate flags for i and reuse the results
      {
        printf("found:  %i x %i = %i\n", i, j, i*j);
        if ( !found[i*j] )
        {
          sum += i*j;
          found[i*j] = 1; 
        }
      }
      j++;
    }
    i++;
  }
  
  printf("raw sum: %i\n",sum);

  free(found);
  
  exit(0);
}
