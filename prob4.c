#include <stdio.h>
#include <stdlib.h>

int is_palindrome(int val)
{

  char buffer[7], reverse[7];
  sprintf(buffer,"%i", val);

  int rez = buffer[0] == buffer[5] 
         && buffer[1] == buffer[4] 
         && buffer[2] == buffer[3];
  if (rez)
  {
    printf("found palindrome: %i\n",val);
    return 1;
  } else {
    return 0;
  }
}

int look_for(int i, int j)
{
  int ij = i*j;
  if (ij < 100000)
    return 0;
  if( is_palindrome(ij) ) {
    return ij;
  } else {
    int a = look_for(i-1,j);
    int b = i < j ? look_for(i,j-1) : 0;
    return a > b ? a: b;
  }
}


int main (void)
{

  printf( "%i\n", look_for(999,999) );

  exit(0);

}
