#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


static inline char* write_hexspace(char* str_index) {
	*(str_index++) = '%';
	*(str_index++) = '2';
	*(str_index++) = '0';
	return str_index;
}


static inline void copy_into(char* from, char* into) {
	while( *(into++) = *(from++) );
}


void replace_space(char* str) {
	int offset = 0, swap = 0;
	
	while( *str ) 
	{
		if( isspace( *str) ) {
			offset += 3;
			swap   += 1;
			do {
				str++;
				offset -= 1;
			} while( isspace( *str ) );
		} else {
			str++;
		}
	}
	
	while( swap ) {
		if( isspace(*str) ) {
			if( isspace(*(str-1)) ) {
				offset++;
			} else {					
				offset -= 2;
				write_hexspace(str+offset);
				swap --;			
			}
		} else {
			*(str+offset) = *str;
		}
		str--;
	}
	
}

void replace_space_and_compress(char* str) {
	int offset = 0, swap = 0;
	while( *str ) 
	{
		if( isspace( *str) ) {
			char* depth_end = str;
			while( isspace( *depth_end) ) depth_end++;
			if( *depth_end == '\0' ) {
				*str = '\0';
				return;
			}
			int depth = depth_end - str;
			if( depth >= 3 ){
				str = write_hexspace( str );
				if( depth > 3 )
					copy_into( depth_end, str ); 
			} else {
				swap += 1;
				offset = 3-depth;
				str += depth;
			}
		} else { str++; }
	}	
	
}


int main(int argc, char* argv[]){

	if(argc > 1)
	{
		replace_space_and_compress(argv[1]);	
		//replace_space(argv[1]);	
		printf("%s|stop|\n",argv[1]);
	}

	exit(0);
	
}



