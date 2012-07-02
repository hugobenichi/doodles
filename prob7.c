#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int is_prime(int value) 
{

  int prime = value % 2 && value % 3;

  if( prime )
  {
    int root = value / 2; 
      
    int k = 6;
    while(k < root)
    {
      if(  !(value % (k-1)) || !(value % (k+1)) )
        return 0;
      k += 6;
    }
  }
  
  return prime;

}


int main(int argc, char* argv[])
{

  int index = 1;
  int n = 2;
  while(index < 10000)
  {
    n++;
    if (is_prime(n))
      index++;
  }
  
  printf("%i\n",n);
  exit(0);

}
