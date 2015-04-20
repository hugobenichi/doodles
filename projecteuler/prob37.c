#include <stdio.h>
#include <stdlib.h>


void sleeve(int* table, int len)
{
  int i = 0;
  while(len-i)
    table[i++] = 1;    
  table[0] = 0;
  table[1] = 0; 
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
  int i = 11, to_find = 11, sum = 0;
    
  int* table = malloc(sizeof(int) * len);

  sleeve(table, len);
  
  int lim = 1;
  while( to_find && len-i )
  {
    int j, rez = 1;
 
    j = i;
    while(rez)
    {
      if ( j == 0)    break;
      if( !table[j] ) rez = 0;
      j /= 10;
    }
    
    j = i;
    while( lim < i ) lim *= 10;
    
    int div = lim /10;
    while( rez && div > 1 )
    {
      if( !table[i%div] )  rez = 0;
      div /= 10;
    }
    
    if (rez)
    {
      printf( "found %i\n", i );
      sum += i;
      to_find--;
    }
        
    i++;
  }
  
  printf("i: %i\nleft to find: %i\nsum: %i\n", i, to_find, sum);

  free(table);

  exit(0);

}
