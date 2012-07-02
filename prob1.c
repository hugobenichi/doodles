#include <stdlib.h>
#include <stdio.h>

int mult_sum(int mult, int stop)
{
  int x = mult; 
  int s = 0;
  while( x < stop )
  {
    s += x;
    x += mult;
  }
  
  return s;

}

int main(int argc, char* argv[])
{

  int a = 3, b = 5, ab = 15;
  int stop = 1000;
  
  if( argc - 1)
    stop = atoi( argv[1]);

  printf("%i\n", mult_sum(a, stop) + mult_sum(b, stop) - mult_sum(ab, stop) );

}
