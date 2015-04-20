#include <stdlib.h>
#include <stdio.h>
#include <math.h>

long is_prime(long value) 
{

  long prime = value % 2 && value % 3;

  if( prime )
  {
    long root = value / 2; 
      
    long k = 6;
    while(k < root)
    {
      if(  !(value % (k-1)) || !(value % (k+1)) )
        return 0;
      k += 6;
    }
  }
  
  return prime;

}

long highest_div(long value) 
{

  long k = value / 12;
      k = 6*(k+1);
      
  long stop = sqrt( value ); // /2;
  
  k = 0;
  while( k < stop) k += 6;
  
  printf("%li\n",k);
  
  while( k > 0)
  {
  //printf("%li\n",k);
/*
    if( !( value % (k+1) ) &&  is_prime(k+1) )
      return k+1;
    if( !( value % (k-1) ) && is_prime(k-1) )
      return k-1;
*/      
   if( !( value % (k+1) ) )
   { 
      printf("%li\n",k+1);
      if ( is_prime(k+1) )
        return k+1;
   }
    if( !( value % (k-1) ) ) 
    {
      printf("%li\n", k-1  );
      if ( is_prime(k-1) )
        return k-1;
    } 
    k -= 6;
  }
  
  return 1;

}


int main(int argc, char* argv[])
{
  
  long max = 600851475143;
  
  printf("%li\n", highest_div( max ) );
  
  exit(0);

}
