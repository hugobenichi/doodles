#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void print_bits(int x){
  uint32_t y     =  (uint32_t)x;
  uint32_t mask  = 1 << 31;
  
  char string[34];
       string[32] = '\n';
       string[33] = '\0';
  char *s = string;
  while(mask) {
  	*s++ = y&mask ? '1' : '0';
    mask = mask >> 1;
  }
  printf("%s",string);
}


int max(int x, int y){
	// return x + (y-x)& ( (x-y)>>31  );  /* does not work if overflow */
	
	int sign_x = x >> 31;
	int sign_y = y >> 31;
	int same_sign = ~( sign_x ^ sign_y );
	int k = (~sign_x & ~same_sign) | ( ((y-x)>>31) & same_sign );
	return y + ( (x-y) & k );
}

int main(void){
	int x = 1<<30;
	int y = -x;
	printf("max(%i,%i) = %i\n", x, y, max(x,y) );
}
