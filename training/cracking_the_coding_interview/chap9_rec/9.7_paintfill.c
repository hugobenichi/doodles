#include <stdio.h>
#include <stdlib.h>

void print_screen(int len, int* screen){

	char* buffer = malloc( sizeof(char) * (len+2) );
	buffer[len+0] = '\n';
	buffer[len+1] = '\0';
	
	for(int line = 0; line < len; ++line) {
		for(int colm = 0; colm < len; ++colm)
			buffer[colm] 
			  = screen[line*len + colm] + '0';
		printf("  %s", buffer );
	}
	free(buffer);
}


void fill_paint(int len, int* screen, int y, int x, int before, int after){
	if( x >= 0 && y >= 0 && x < len && y < len && screen[y*len+x] == before ) {
		screen[y*len+x] = after;
		fill_paint(len, screen, y-1, x, before, after);
		fill_paint(len, screen, y, x-1, before, after);
		fill_paint(len, screen, y+1, x, before, after);
		fill_paint(len, screen, y, x+1, before, after);
	}
}

int main(int argc, char* argv[]) {

	int len = 10;
	int screen[100] = {
	 0,0,0,0,0,4,0,0,0,0, 
	 0,0,0,0,0,4,0,0,0,0, 
	 0,0,0,0,0,4,0,0,0,0, 
	 0,0,0,0,4,0,0,0,0,0, 
	 0,4,4,4,0,0,0,0,0,0, 
	 4,0,0,0,0,3,3,0,0,0, 
	 0,0,0,0,3,0,0,3,0,0, 
	 0,0,0,0,3,0,3,0,0,0, 
	 0,0,0,0,3,0,3,3,0,0, 
	 0,0,0,0,3,0,0,0,3,0
	};

	int x = 4, y= 5, c = 3;

	if(argc > 3){
		x = atoi(argv[1]) % 10;
		y = atoi(argv[2]) % 10;
		c = atoi(argv[3]) % 10;
	}
	
	printf("\n\n");
  print_screen(len, screen);
  
  fill_paint(len, screen, y, x, screen[y*len+x], c);
  
	printf("\n\n");
  print_screen(len, screen);

	exit(0);
}
