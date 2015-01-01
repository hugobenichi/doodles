#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int total = 0;

int count_steps(int x, int y, int x_lim, int y_lim){
	total++;
	if( x == x_lim || y == y_lim )
		return 1;
	return count_steps( x+1, y, x_lim, y_lim )
       + count_steps( x, y+1, x_lim, y_lim );
}


int count_steps2(int x, int y, int x_lim, int y_lim){

	int* mem = malloc( sizeof(x) * (x_lim+1)*(y_lim+1) );
	assert(mem);
	for(int i = 0; i < (x_lim+1)*(y_lim+1); i++)
		mem[i] = -1;

	int steps_rec(int xx, int yy, int xx_lim, int yy_lim){
		if( xx == xx_lim || yy == yy_lim )
			return 1;
		if( mem[xx + yy * (xx_lim+1)] < 0 ) {
			total++;
			mem[xx + yy * (xx_lim+1)] = steps_rec( xx+1, yy, xx_lim, yy_lim )
                                + steps_rec( xx, yy+1, xx_lim, yy_lim );
		}
		return mem[xx + yy * (xx_lim+1)];
	}
	
	int rez = steps_rec(0,0,x_lim,y_lim);
	free(mem);
	return rez;
}


int main(int argc, char* argv[]) {

	if(argc>1) {
		int i = atoi(argv[1]);
		assert( i > 0);
		//int count = count_steps(0,0,i,i);
		//printf("%i  (calls:%i)\n", count, total);
		total = 0;
		int count2 = count_steps2(0,0,i,i);
		printf("%i  (calls:%i)\n", count2, total);
	}

	exit(0);
}
