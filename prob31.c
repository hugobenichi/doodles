#include <stdio.h>
#include <stdlib.h>

static int calls = 0;

int counts(int tot, int* coins, int highest) {
	calls++;

	if( tot <  0) return 0; 
	if( tot == 0) return 1;
	
	int sum = counts(tot - coins[highest], coins, highest );
	
	if(highest > 0)
			sum += counts(tot, coins, highest-1 );

	return sum;
}

int main(int argc, char* argv[]) {

	int coins_nbr = 8; 
	//int coins[5]  = {1,2,5,10,25};
	int coins[8]  = {1,2,5,10,20,50,100,200};
	int total     = 200;
	
	if(argc>1) total = atoi(argv[1]);
	
	int combination = counts(total, coins, coins_nbr-1);

	printf("%i  (%i)\n", combination, calls );
		
	exit(0);
}
