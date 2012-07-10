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
  
  int i = 1;
  int sum = 0;
  int tot = 0;
  int bst_tot = 0;
  int bst_sum = 0;
  while( i < len )
  {
    do { i++; } while( i < len && !table[i] );
    if ( i >= len ) break;    
//printf(" prime start: %i\n",i); 
    sum = i;
    tot = 1;
    int j = i;
    
    while( sum < len )
    {

      if ( table[sum] && bst_tot < tot) 
      {
        bst_tot = tot;
        bst_sum = sum;
      }   
 
      do{ j++; } while( j < len && !table[j] );
      if ( j >= len ) break;

      sum += j;
//printf("   next: %i | sum: %i\n", j, sum);       
      tot += 1;
      
   
      
    }
    

    
  }
//printf("%i\n",i);

  free(table);
  
  printf("prime: %i is the sum of %i primes\n",bst_sum, bst_tot);

  exit(0);

}
