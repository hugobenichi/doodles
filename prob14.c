#include <stdio.h>
#include <stdlib.h>


int main(void)
{

  int max = 1000000;
  long table_lim = max * 10000;
  
  int* length_table = malloc( sizeof(int) * table_lim);

  int k = 0;
  while(max-k) 
    length_table[k++] = 0;
  
  
  k = 1;
  while(max-k)
  {
    int step = 0;
    long n = k;
//printf("%i:", k);
    while(1)
    {
      //if( n > table_lim )
      //  printf("overflow for starting %i: %li\n", k, n);
      if( n == 1)
      {
        length_table[k] = step + 1;
        break;
      }
      if(n < table_lim && length_table[n] )
      {
        length_table[k] = length_table[n] + step;
        break;
      }
      if( (n & 1) )
        n = 3*n+1;
      else
        n = n/2;
//printf("%i ", n);
      step++;
    }
    
//printf("->%i\n", length_table[k]);
    k++;
  }
  
  k = 0;
  int longest = 1;
  int longest_i = 0;
  while(max-k)
  {
    if( length_table[k] > longest)
    {
      longest = length_table[k];
      longest_i = k;
    }
    k++;
  }
  printf("longest chain starting from %i, length %i\n", longest_i, longest); 

  free(length_table);

  exit(0);
}
