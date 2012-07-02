#include <stdlib.h>
#include <stdio.h>

int is_abundant(int value)
{
  int sum = 0;
  int i = 1;
  while(value/2+1 - i)
  {
    if ( 0 == (value % i) )
      sum += i;
    i++;
  }
  return sum > value;
}

int main(void)
{
  
  int i, j, sum, lim = 28124;
  
  int* abundant        = malloc( sizeof(int) * lim );
  int* abundant_notsum = malloc( sizeof(int) * lim ); 
  
  i = 1;
  while(lim-i)
  {
    abundant[i] = is_abundant(i);
    abundant_notsum[i] = 1;
    i++;
  }
  
  i = 1;
  while(lim-i)
  {
    if( abundant[i] ) 
    {    
      j = 1;
      while(lim-j)
      {
        if( abundant[j] && (i+j < lim) )
          abundant_notsum[i+j] = 0;       
        j++;
      }        
    }
    i++;
  }
  
  sum = 0;
  i = 1;
  while(lim-i)
  {
    if ( abundant_notsum[i] )
      sum += i;
    i++;
  }
  
  printf("%i\n", sum);
  
  free(abundant);
  free(abundant_notsum);
    
  exit(0);
}
