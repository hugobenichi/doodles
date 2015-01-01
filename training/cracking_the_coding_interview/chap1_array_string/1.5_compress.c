#include <stdio.h>
#include <stdlib.h>

void compress(char* str) {
	char* compress = str;
	while(*str) {
		int count = 0;
		char letter = *str;
		while( *str == letter ) {
			count++;
			str++;
		}
		*compress++ = letter;
		if(count > 1)
			*compress++ = count + '0';
	}
	*compress = '\0';
}

int main(int argc, char* argv[]) {

	if(argc > 1) {
		compress(argv[1]);
		printf( "%s\n",argv[1]);
	}
	
	exit(0);

}
