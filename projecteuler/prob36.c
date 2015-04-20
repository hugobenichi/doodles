#include <stdio.h>
#include <stdlib.h>

int is_palindrome10(int val)
{
  int ori = val;
  int rev = 0;
  while( val > 0)
  {
    rev  = 10 * rev + val % 10;
    val /= 10;
  }
  return ori == rev;
}

unsigned int flip2(unsigned int val)
{
  unsigned int flp = 0, i;
  for ( i = 0; i < 21; i++ )
  {
    flp += val & 0x0000001;
    flp <<= 1;
    val >>= 1;
  }
  return flp;
}

unsigned int is_palindrome2(unsigned int val)
{
  unsigned int flp = flip2(val);
  while( ! (flp & 0x00000001) )
    flp >>= 1;
  return flp == val;
}

int main (void)
{

  printf("%i\n", is_palindrome10(585) );

  unsigned int sum = 0, i = 1, lim = 1000000;
  
  while( lim - i )
  {
    if( is_palindrome2(i) && is_palindrome10(i) ) 
      sum += i;
    i++;
  }

  printf("%i\n", sum);

  exit(0);
}

