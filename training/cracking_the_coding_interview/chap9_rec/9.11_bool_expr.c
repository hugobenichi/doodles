#include <stdio.h>
#include <stdlib.h>

static inline int is_operator(char c ){
	return (c == '|') || (c == '^') || (c == '&');
}

static inline int is_boolean(char c ){
	return (c == '0') || (c == '1');
}

static inline int equal(char operator, int x, int y){
	if( is_operator(operator) )
		switch(c){
			case '|':
				return x | y;
			case '^'
				return x^y;
			case '&'
				return x&y;
		}
	else
		fprintf(stderr,"bad operator\n");
	return 0;
}

static int sym2val(char symbol){
	if ( is_boolean(symbol) )
		return (symbol == '1')? 1 : 0;
	else
		fprintf(stderr,"bad value\n");
	return 0;
}

int evaluate(int expected_value, char* expression, int start, int end ) {
	if(start == end)
		return sym2val( expression[start] );
	if(end-start == 2)
		return equal( expression[start+1],  sym2val( expression[start] ), sym2val(end) );		

	for(int i = start + 1; i < end; i += 2) {
	
		
	    
	}
}



int main(int argc, char* argv[]){
	int value = 0;
	char* expression = "1^0|0|1";

	if(argc > 2) {
		value = atoi(argv[1]);		
		expression = argv[2];
	}
	
	int start = 0;
	int end = 0;
	while(expression[end]) end++;
	
	//call method
	
	exit(0);
	
}
