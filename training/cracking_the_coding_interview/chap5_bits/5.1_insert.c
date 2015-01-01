#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>   
#include <assert.h>

// handy for verification
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

// question 5.1
int insert(int bits, int into, int from, int to){  
	assert(from > 0);
	assert(to > 0);
	int mask = (~0)<<(to+1) ^ (~0)<<(from);   // it will neatly do nothing if from > to
	return (into & ~mask) | ( (bits<<from) & mask);
}

// question 5.5
void count_swap(int n, int m){
		int count = 0;
		for(int nm = n^m; nm != 0; nm = nm >> 1) if ( nm & 1 ) count++;
		print_bits(n^m);
		printf("%i\n",count);
}

// question 5.6
int swap_odd_even(int n) {
	return  (n & 0xAAAAAAAA)>>1 | (n & 0x55555555)<<1 ;
}

int main(int argc, char* argv[]){
  	
	if ( 0 && argc > 4) { 
		int m = atoi(argv[1]), n = atoi(argv[2]), i = atoi(argv[3]), j = atoi(argv[4]);
		print_bits(n);
		print_bits(m);
		print_bits( insert(m,n,i,j) );
	}
	
	if ( 0 && argc > 2) { 
		int m = atoi(argv[1]), n = atoi(argv[2]);
		print_bits(n);
		print_bits(m);
		count_swap(n,m);
	}
	
	if ( 1 && argc > 1) { 
		int n = atoi(argv[1]);
		print_bits(n);
		print_bits( swap_odd_even(n) );
	}	
  	
  exit(0);
}
