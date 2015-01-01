#include <stdio.h>
#include <stdlib.h>

void reverse_str(char* str) {
	char* end = str;
	while( *end ) end++;
	end--;
	while( end - str >= 0 ) {
		char swap = *str;
		*str++ = *end;
		*end-- = swap;
	}
}


int main(int argc, char* argv[]) {
	
	if( argc > 1)
	{
		reverse_str( argv[1] );
		printf("%s\n", argv[1] );
	}

	exit(0);

}
