#include <stdio.h>
#include <stdlib.h>

int unikchar(char* str) {
	int charcount[256] = {0};

	while(*str) {
		int index = (unsigned char)*str;
		if( index != 32) {
			if(charcount[index]) return 0;
			charcount[index]++;
		}
		str++;
	}
	return 1;
}


int main(int argc, char* argv[]) {
	
	if( argc > 1)
	{
		printf("%s\n", unikchar( argv[1] )? "yes" : "no" );
	}

	exit(0);

}
