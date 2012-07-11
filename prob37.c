#include <stdio.h>
#include <stdlib.h>

void sleeve(int* table, int len)
{
  int i = 0;
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

  int len = 1000000;
  int* table = malloc(sizeof(int) * len);

  sleeve(table, len);
  table[0] = 0;
  table[1] = 0;
  
  int i = 11, to_find = 11, sum = 0;

if (0)
{  
  int x = 3797;
  int lim = 1;
  while (lim < x) lim *= 10;
  lim /= 10;
  while( lim > 1)
  {
    printf(">> %i\n", x % lim);
    lim /= 10;
  }
}
  
  int lim = 1;
  while( len-i )
  {
  
    int j = i;
    int rez = 1;
//printf("  i: %i\n",i);
    while( 1 )
    {
      if ( j == 0)
        break;
    
      if( !table[j] )
      {
        rez = 0;
        break;
      }
      
      j /= 10;
//printf("  j: %i\n",j);
      
    }
    
    j = i;
    while( lim < i ) lim *= 10;
    
    int lim2 = lim /10;
    while( lim2 > 1 )
    {
      if( !table[ i % lim2 ] )
      {
        rez = 0;
        break;
      }
      lim2 /= 10;
    }
    
    if (rez)
    {
      printf( "found %i\n", i );
      to_find--;
      sum += i;
      if( !to_find ) break;
    }
        
    i++;
  }
  
  printf("i: %i\nleft to find: %i\nsum: %i\n", i, to_find, sum);

  free(table);

  exit(0);

}
