#include <stdio.h>
#include <stdlib.h>

int isanagram(char* a, char* b) {
	int len = 0;
	int charcount[256] = {0};

	while(*a) {
		int index = (unsigned char)*a;
		if( index != 32) {
			charcount[index] += 1;
			len++;
		}
		a++;
	}

	while(*b) {
		int index = (unsigned char)*b;
		if( index != 32) {
			if ( charcount[(unsigned char)*b] )
				charcount[(unsigned char)*b] -= 1;
			else
				return 0;
			len--;
		}
		b++;
	}
	if (len) return 0;
	return 1;
}


int main(int argc, char* argv[]) {
	
	if( argc > 2)
	{
		printf("%s\n", isanagram( argv[1], argv[2] )? "yes" : "no" );
	}

	exit(0);

}
