#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

void print_bits(int x){
  uint32_t y     =  (uint32_t)x;             // seems like direct conversion works ok
  //uint32_t y     = *( (uint32_t*)  &x  ); 
  uint32_t mask  = 1 << 31;
  
  char string[34]; // char array buffer allow for a single call to printf.
       string[32] = '\n';
       string[33] = '\0';
  char *s = string;
  while(mask) {
  	*s++ = y&mask ? '1' : '0';
    mask = mask >> 1;
  }
  printf("%s",string);
}

int count_ones(int n){
	int count = 0;
	for(uint32_t m = n; m != 0; m = m >> 1) if ( m & 1 ) count++;
	return count;
}

int count_bit(int val)
{

  int bit_left = 32;
  int total = 0;
	
  while(bit_left)
  {
    if ( val & 1 )
      total++;
    val = val >> 1;
    bit_left--;
  }

  return total;

}

int main(int argc, char* argv[])
{
	
  if( argc > 1)
  {
    int i = atoi(argv[1]);
    printf("%i -> %i bits\n", i, count_bit(i));
    
unsigned int r;
int const mask = i >> sizeof(int) * CHAR_BIT - 1;

print_bits(i);
print_bits(mask);
print_bits(i+mask);

r = (i + mask) ^ mask;
  
  printf("%i\n",r);  
    
  }
  
  exit(0);

}
