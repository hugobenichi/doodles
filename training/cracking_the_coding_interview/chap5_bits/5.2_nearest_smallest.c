#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>   
#include <assert.h>

// handy for verification
void print_bits(int x){
  uint32_t y     =  (uint32_t)x;             // seems like direct conversion works ok
  //uint32_t y     = *( (uint32_t*)  &x  ); 
  uint32_t mask  = 1 << 31;  // change 31 by sizeof(int)*CHAR_BIT - 1
  
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

// brute force
int nearest_small(int n) {
	int count = count_ones(n);
	if( (count==32) || (count==0) )
		return n;
	if( ~(-1<<count) == n )
		return n;
	n--;
	while( n > 0 ) {
		if( count_ones(n) == count)
			return n;
		n--;
	}
	return 0;
}

// find first 0; then first first one after 0 <- int first
// count numbers of 1 until this first one, not included <- int ones
// swap the first 10 to 01
// fill out the lower bits with as many ones, starting from the left
int nearest_small2(int n) {
	int count = count_ones(n);
	if( (count==32) || (count==0) )
		return n;
	if( ~(-1<<count) == n )
		return n;
	
	int ones  = 0;	
	int first = 1;
	int mask  = 1;
	while( 0 != (n & first) ){
		first = first << 1;
		mask = 1 + ( mask << 1);
		ones++;
	}
	while( 0 == (n & first) ) {
		first = first << 1;		
		mask = 1 + ( mask << 1);
	}
	
	first = first >> 1;
	int tail = first;
	while(ones) {
		first = first >> 1;
		tail = tail | first;
	  ones--;
	}
		
	return ( n & ~mask ) | tail; 
}

// brute force
int nearest_large(int n) {
	int count = count_ones(n);
	if( (count==32) || (count==0) )
		return n;
	n++;
	while( 1 ){
		if( count_ones(n) == count)
			return n;
		n++;
	}
	return 0;
}

// locate first 1, and then first 0 after series of 1
// write 1 at the location of the first 0 after series of 1
// write 0 one bit on the right
// shift remaining tail bits to the right as long as LSB is 0
int nearest_large2(int n) {
	if ( n == 0 ) return 0; 
	int upper_zer = 1;
	int shift = 0;
	while( 0 == (n & upper_zer) ) {
		upper_zer = upper_zer << 1;
		shift++;
	}
	while( 0 != (n & upper_zer) ) {
		upper_zer = upper_zer << 1;		
		shift++;
	}
	
	int mask_tail = ~( -1 << (shift-1) );
	int mask_head =  ( -1 << (shift+1) );
	
	int tail = n & ( mask_tail ) ;
	if( tail != 0 ) // important to avoid infinite loops of nul tails
		while( 0 == (tail&1) )
			tail = tail>>1;
		
	return (n&mask_head) | upper_zer | tail;
}

int main(int argc, char* argv[]){
	
	if ( 1 && argc > 1) { 
		int n = atoi(argv[1]);
	
		int small = nearest_small(n);
		printf( "%.4i(%.4i)  ", small, count_ones(small) );	
		print_bits(small);
		
		int small2 = nearest_small2(n);
		printf( "%.4i(%.4i)  ", small2, count_ones(small) );	
		print_bits(small2);
		
		printf( "%.4i(%.4i)  ", n, count_ones(n) );			  
		print_bits(n);
		
		int large = nearest_large(n);
		printf( "%.4i(%.4i)  ", large, count_ones(large) );	
		print_bits(large);
		
		
		int large2 = nearest_large2(n);
		printf( "%.4i(%.4i)  ", large2, count_ones(large2) );	
		print_bits(large2);
	}	
  	
  exit(0);
}
