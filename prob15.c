#include <stdio.h>
#include <stdlib.h>


int main(void)
{

  int length = 21;
  int g_size = length*length;
  
  long* grid = malloc( sizeof(long) * g_size );

  grid[0] = 0;
  grid[length*1 + 0 ] = 1;
  grid[length*0 + 1 ] = 1;
  grid[length*1 + 1 ] = 2;

  int k, i, h, tot;
  
  k = 2;
  while( length - k )
  {
    i = 0;
    while(length - i)
    {
    
    /*
printf("for i=%i\n",i);
      h = 0;
      tot = 0;
      while( i >= h )
      {
printf("  h=%i  ",h);
        tot += (1+i-h) * grid[length*(k-1) + h];
printf(" found  %i x %i\n",(1+i-h), grid[length*(k-1) + h]);
        h++;
      }
      grid[length*k + i] = tot;
      grid[length*i + k] = tot;
      
      */
      
      h = i;
      grid[length*k + i] = 0;
      while(h >= 0)
      {
        grid[length*k + i] += grid[length*(k-1) + h];
        h--;
      }
        
        
        
      grid[length*i + k] = grid[length*k + i];
      i++;
    }
    k++;
  }
  
  /* print all */ 
  
  int index = 0;
  k = length-1;
  while(k >= 0)
  {
    i = length-1;
    while(i >= 0)
    {
      printf("%.10li  ", grid[length*k+i] );
      i--;
    }
    printf("\n\n");
    k--;
  }
  
  printf("\n%li\n",grid[g_size-1]);


  free(grid);
  exit(0);

}
