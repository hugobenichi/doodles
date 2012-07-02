#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

  int max = atoi(argv[1]);

  int sum = max * (max+1) / 2;
  int sum_square = 0;

  int i;
  for( i = 0; i < max+1; i++)
    sum_square += i*i;
  
  printf("%i\n", sum_square - sum*sum);
  
  exit(0);
}
