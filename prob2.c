#include <stdlib.h>
#include <stdio.h>



int main(int argc, char* argv[])
{

  int a = 1,
      b = 2,
      c;

  int msk  = 1;
  int sum  = 0.0;
  int stop = 4000000; 
  
  if (argc > 1) 
    stop = atoi(argv[1]);

  while( b < stop)
  {
    if ( !( b & msk ))
      sum += b;
    c = b;
    b += a;
    a = c;
  }
  
  printf("%i\n",sum);

  exit(0);

}
