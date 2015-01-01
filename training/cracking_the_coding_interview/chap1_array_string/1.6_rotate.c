#include <stdio.h>
#include <stdlib.h>


static inline void swap(int *mat, int i, int j){
	int temp   = mat[i];
			mat[i] = mat[j];
			mat[j] = temp;
}

void print_mat(int *mat, int len ) {
	char* buffer  = malloc(sizeof(char)*(len+2));
	buffer[len+0] = '\n';
	buffer[len+1] = '\0';
	
	for(int i = 0; i < len; ++i) {
		for(int j = 0; j < len; ++j) {
			//buffer[j] = (char)(mat[j + i*len]) - '0'; 
			printf("%.1i ",mat[j + i*len]);
		}
		printf("\n");
		//printf( "%s", buffer );
	}
	free(buffer);
}
/*
void rotate_mirror(int *mat, int len){
  for(int outer = 0; outer < len; ++outer) {
  	for(int inner = 0; inner < len/2; ++inner) {
      swap( mat, outer*len+inner, outer*len+len-1-inner );
    }
  }
}
*/
void rotate_mirror(int *mat, int len){
	int outer = 0;
	int i_index = 0;
	while(len-outer) {
		int inner = 0;
  	while(len/2-inner){
      swap( mat, i_index+inner, i_index+len-1-inner );
      inner++;
    }
    i_index += len;
    outer++;
  }
}

void rotate_diag(int *mat, int len){
	int outer = 0;
  while(len-outer) {
  	int inner = outer;
  	while(len-inner) {
      swap( mat, len*(len-1-inner) + outer, len*(len-1-outer) + inner );
      inner++;
    }
    outer++;
  }
}

void rotate_direct(int *mat, int len){
  for(int outer = 0; outer < len/2; ++outer) {
    int first = outer;
    int last = len - 1 - outer;
  	for(int inner = first; inner < last; ++inner) {
			int offset = inner - first;
			int temp = mat[first*len+inner];

			mat[first*len+inner]   				= mat[(last-offset)*len+first];
			mat[(last-offset)*len+first]  = mat[last*len+last-offset];
			mat[last*len+last-offset]    	= mat[inner*len+last];
			mat[inner*len+last] 					= temp;
    }
  }
}

int main(int argc, char* argv[]) {

	int len = 4;
	if(argc>1) len = atoi(argv[1]); 

	int *mat = malloc( sizeof(int) * len*len );

	for(int i; i < len*len; ++i)
		mat[i] = i % 10;

	//printf("\n\n");
	//print_mat(mat,len);
	//printf("\n\n");

	rotate_mirror(mat,len);
	rotate_diag(mat,len);

	//print_mat(mat,len);
	//printf("\n\n");

	//for(int i; i < len*len; ++i)
	//	mat[i] = i % 10;

	rotate_direct(mat,len);

	//print_mat(mat,len);
	//printf("\n\n");
		
	free(mat);

	exit(0);
}
