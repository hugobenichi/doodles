

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
  int i;
  int max = 2000000;

  int* prime_table = malloc( sizeof(int) * max );
  for( i = 0; i < max; i++)
    prime_table[i] = 1;
    
  
  i = 2; 
  while( i < max )
  {
    if( prime_table[i] )
    {
      int k = 2*i;
      while( k < max)
      {
        prime_table[k] = 0;
        k += i;
      } 
    }
    i++;
  }
  
  long sum = 0;
  i = 2;
  while(i < max)
  {
    if(prime_table[i])
      sum += i;
    i++;
  }
  
  printf("%li\n", sum);
  
  free(prime_table);
  
  exit(0);

}






