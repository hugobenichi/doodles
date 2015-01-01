#include <stdio.h>
#include <stdlib.h>

void print_screen(char* screen, int byte_width, int length) {
	int width  = byte_width * 8;
	int height = length / width;

	char* line = malloc( sizeof(char) * width + 2); //number of pixels in one line + '\n\0'
	line[width]     = '\n';
	line[width + 1] = '\0';
	
	int i = 0;
	int screen_index = 0;
	while( i < height ){
		int j = 0;
		int line_index=0;
		while( j < byte_width ) {
			int mask = 1<<7;
			while(mask != 0) {
				line[line_index] = mask & screen[screen_index] ? '1' : '0';
				mask = mask >> 1;
				++line_index;
			}
			printf("%s", line);
			++j
			++screen_index;
		}	
		++i;
	}
	
	free(line);
}


int main(void) {

	int width  = 2; // * 8 pixels 
	int height = 16;
	int length = width * height; 
	char screen[2*16] = {0};
	
	print_screen( screen, width, length);
	//print_screen( screen, width, length);
	
	exit(0);
}
