#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void mat_del(int* m, int nlen, int mlen) {

	int* i_coords = malloc( sizeof(int) * nlen);
	int* j_coords = malloc( sizeof(int) * mlen);
	
	assert(i_coords); //should have allocated memory
	assert(j_coords); //idem
	
	int i_ind = 0, j_ind = 0;
	int i, j;
	for( i = 0; i  < nlen; i++) {
		for( j = 0; j < mlen; j++) {
			if(m[i + j*nlen] == 0)
			{
			  i_coords[i_ind++] = i;
			  j_coords[j_ind++] = j;
			}
		}
	}
	
	while( --i_ind >= 0) {
		for( j = 0; j < mlen; j++)
			m[i_coords[i_ind] + j*nlen] = 0;	
	}
		
	while( --j_ind >= 0) {
		for( i = 0; i < nlen; i++)
			m[i + j_coords[j_ind]*nlen] = 0;	
	}
	
	free(i_coords);
	free(j_coords);
	
}


int main(int argc, char* argv[]) {

	int mat[4*3] = { 1, 2, 3, 0, 5, 0, 7, 8, 9, 10, 11, 12}; //, 13, 14, 15, 16 };
	
	printf("%2i %2i %2i %2i\n", mat[0], mat[1], mat[2], mat[3]);
	printf("%2i %2i %2i %2i\n", mat[4], mat[5], mat[6], mat[7]);
	printf("%2i %2i %2i %2i\n\n", mat[8], mat[9], mat[10], mat[11]);
	
	mat_del(mat, 4, 3);
	
	printf("%2i %2i %2i %2i\n", mat[0], mat[1], mat[2], mat[3]);
	printf("%2i %2i %2i %2i\n", mat[4], mat[5], mat[6], mat[7]);
	printf("%2i %2i %2i %2i\n", mat[8], mat[9], mat[10], mat[11]);

	exit(0);

}
