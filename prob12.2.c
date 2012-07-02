#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n_div(long val, long root){
  int i = 2;
  int ncount = 2;
  while(i <= root)
  {
    if( 0 == (val%i) )
      ncount += 2;
    i++;
  }
  if( root*root == val && root != 1)
    ncount--; 
//printf("%li (root: %li): %i (max index: %i)\n",val,root,ncount,i);
  return ncount;
}

int n_div_odd(long val, long root){
  int i = 3;
  int ncount = 2;
  while(i < root)
  {
    if( 0 == (val%i) )
      ncount +=2;
    i += 2;
  }
  if( 0 == (val%root) && root != 1 )
    ncount++; 
  return ncount;
}

int main(void) {

  int limit = 500;

  long n = 1;
  int count_n = 1;
  int count_np1 = 2; 
/*  
  while(1)
  {
    n++;
    count_n = count_np1;
    count_np1 = n_div( n+1, sqrt(n+1) );
        
    if( (count_n & 1) && (count_n-1) * count_np1 >= limit )
      break;
    if( !(count_n & 1) && (count_n-2) * count_np1 >= limit )
      break;
      
    n++;
    count_n = count_np1;
    count_np1 = n_div( n+1, sqrt(n+1) );
    
    if( (count_np1 & 1) && count_n * (count_np1 - 1) >= limit )
      break;
    if( !(count_np1 & 1) && count_n * (count_np1 - 2) >= limit )
      break;
      printf("%li: %i div\n", n*(n+1)/2, count_n * count_np1 - 2);
  }
*/

  long i = 1;
  while( count_n < limit )
  {
    i++;
    n += i;
    count_n = n_div(n, sqrt(n) );
  }

  printf("%li: %i div\n", n, count_n );
  
  exit(0);

}
