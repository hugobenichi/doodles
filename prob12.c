#include <stdio.h>
#include <stdlib.h>


int main(void) {

  long i;
  long max = 100000000;
  int limit = 500;
  int up_to = limit * 2;

  int* dividers = malloc( sizeof(int) * max );
  for( i = 0; i < max; i++)
    dividers[i] = 1;
  
  i = 2; 
  while( i < max ) 
  {
    long k = i;
    while( k < max)
    {
      dividers[k] += 1;
      if ( dividers[k] >= up_to )
        max = k;
      k += i;
    } 
    i++;
  }
  
  i = 0;
  long tri = 0;
  while(1)
  {
    i++;
    tri += i;
    if( dividers[tri] >= limit)
      break;
  }
  printf("%lith triangle %li: %i div\n", i, tri, dividers[tri]);
  
  free(dividers);
  
  exit(0);

}
