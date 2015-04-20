#include <stdlib.h>
#include <stdio.h>

int divisors_sum(int value)
{
  int sum = 0;
  int i = 1;
  while(value/2+1 - i)
  {
    if ( 0 == (value % i) )
      sum += i;
    i++;
  }
  return sum;
}

int main(void)
{
  
  int i, sum, lim = 10001;
  
  int* sum_table = malloc( sizeof(int) * lim );
  
  
  i = 1;
  while(lim-i)
  {
    sum_table[i] = divisors_sum(i);
    i++;
  }
  
  sum = 0;
  i = 1;
  while(lim-i)
  {
    if ( sum_table[i] < lim && i == sum_table[sum_table[i]] && i != sum_table[i] )
    {
      printf("found %i and %i\n",i, sum_table[i]);
      sum += i;
    }
    i++;
  }
  
  printf("%i\n", sum);
  
  free(sum_table);
  
  exit(0);
}
