#include <stdio.h>
#include <stdlib.h>

int is_pandigital(int val)
{
  int flags = 0;
  int len = 0;
  int rez = 1;
  
  while(rez && val > 0)
  {
    int bit = 1 << (val % 10) - 1; // 1 at bit index 0
    if( flags & bit )
      rez = 0;
    flags += bit;
    val /= 10;
    len++;
  }
  
  while(rez && len)
  {
    if ( !(flags & 0x0001) )
      rez = 0;
    flags >>= 1;
    len--;
  }
  
  return rez;
}

int main(void)
{
  printf("%i\n", is_pandigital(88) );
  printf("%i\n", is_pandigital(87) );
  printf("%i\n", is_pandigital(75423) );
  printf("%i\n", is_pandigital(453124) );
  
  printf("%i\n", is_pandigital(2143) );
  printf("%i\n", is_pandigital(625314) );

  exit(0);
}
