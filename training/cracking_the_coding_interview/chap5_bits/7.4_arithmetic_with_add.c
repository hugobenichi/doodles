#include <stdio.h>
#include <stdlib.h>

int neg(int x){
	return ~x+1;
}

int sub(int x, int y){
	return x + ~y + 1;
}

int mult(int x, int y){
	int min = y + ( (x-y)&((x-y)>>31) );
	int max = x - ( (x-y)&((x-y)>>31) );
	int rez = 0;
	while(min) {
		rez += max;
		min--;
	}
	return rez;
}

int mult2(int x, int y){  /* also works with negative y but I am not sure why */
	int sign = 0;
	int rez = 0;
	for(int i = 0; i < 32; ++i){
		if( y & 1 )
			rez += x;
		y = y >> 1;
		x = x << 1;
	}
	return rez;
}

int divi(int x, int y){
	int a = 0;
	while( x >= y){
		x -= y;
		a++;
	}
	return a;
}


int main(int argc, char* argv[]) {
	if( argc > 2) {
		int x = atoi(argv[1]);
		int y = atoi(argv[2]);
		printf("%i\n", divi(x,y));	
	}
	exit(0);
}
