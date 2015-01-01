#include <stdio.h>
#include <stdlib.h>

static int total_call = 0;

int count(int length){
	total_call++;
	if( length == 0 )
		return 1;
	if( length < 0 )
		return 0;
	return count(length-1) + count(length-2) + count(length-3);
}

int count2(int length){
	int* mem = malloc(sizeof(length)*(length+1));
	for(int i = 4; i < length+1; i++  )
		mem[i] = -1;
		mem[0] = 0;
		mem[1] = 1;
		mem[2] = 2;
		mem[3] = 4;

	int count_rec(int length) {
		if( length > 3 && mem[length] < 0){ 
			total_call++;
			mem[length] = count_rec(length-3) 
			            + count_rec(length-2)
			            + count_rec(length-1);
		}	
		return mem[length];
	}
	
	int rez = count_rec(length);
	free(mem);
	return rez;	
}

int main(int argc, char* argv[]) {

	if(argc>1) {
		int i = atoi(argv[1]);
		//printf("count1: %i     ", count(i) );		
		//printf("(total call %i)\n",total_call );	
		total_call = 0;	
		printf("count2: %i     ", count2(i) );		
		printf("(total call %i)\n",total_call );	
	}

	exit(0);
}
