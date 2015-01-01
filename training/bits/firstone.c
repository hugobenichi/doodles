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

static inline int first_one(int val){
  return val & (-val);
}

static inline int propagate_right(int val) {
	return val | (val-1);
}

static inline int upper_2power(int val){
	return first_one(1 + propagate_right(val));
}

int next_perm(int x ){
	int head = 1+propagate_right(x);   //remove the first groups of '1's from x
	int tail = x & (first_one(head) >>1) -1;
	if( tail != 0) while( (tail & 1) == 0 ) tail = tail >> 1; 
	return head | tail;
}

int main(int argc, char* argv[])
{
	
  if( argc > 1)
  {
    int i = atoi(argv[1]);
    printf("%i -> %i bits  \n", i, count_ones(i));
    print_bits(i);
  
    int times = 10;
    
    while(times--){
    	i = next_perm(i);
    	printf("%i -> %i bits  ", i, count_ones(i));
    	print_bits(i);
    }
  
  }
  
  exit(0);

}
