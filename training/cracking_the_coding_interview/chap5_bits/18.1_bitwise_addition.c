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


int badd(int x, int y){   // automatically works with two's complement
	int carry = 0;
	int rez   = 0;
	for(int i = 0; i < 32; ++i ) {
		int digit = (x^y^carry)&1;
		    carry = ( x&y | x&carry | y&carry )&1; 
		rez = rez | (digit << i);
		x = x >> 1;
		y = y >> 1;
	}

	return rez;
}

int main(int argc, char* argv[]){
	if(argc > 2) {
		int a = atoi(argv[1]);
		int b = atoi(argv[2]); 
		printf("%i + %i = %i\n", a, b, badd(a,b) );
	
	}
}
