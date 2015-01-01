#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void print_bits(int x);

int get_bit(int i, int j){
	return (i >> (j-1)) & 1;
}

int missing(int *ary, int length){
	int rez = 0;
	int mask = 0;

	for(int j = 1; j < 6; ++j) {
		int count_zer = 0, count_one = 0;	
		for(int k = 0; k < length; ++k) {
			int x = ary[k];
			if( (mask & x) == rez )
				if( get_bit( x, j ) )
					count_one++;
				else
					count_zer++;
		}	
		//printf("%ith bit,  %i '0's and %i '1's -> %c\n", 
		//  j, count_zer, count_one, (count_one < count_zer)? '1' : '0');
		  
	  if( count_one < count_zer) 
			rez |= 1 << (j-1);	
			
	 	mask = (mask<<1) + 1;
	}
	
	return rez;
}


#include <time.h>

int main(int argc, char* argv[]) {
	srand(time(NULL));
	if( argc > 1) {
		int len = atoi(argv[1]);
		int miss = rand() % len;
		int* array = malloc(sizeof(int)*len);
		int k = 0;
		
		for(int i = 0; i < len+1; ++i)
			if( i != miss )
				array[k++] = i;
				
		int rez = missing(array, len);
		printf("miss:  %i -> ",miss); print_bits(miss);
		printf("found: %i\n", rez );
	}
	exit(0);
}

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

