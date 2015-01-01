#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

void print_ary(int *ary, int len);
void print_range(int offset, int start, int end, int width);
void print_middle(int offset, int mid, int width);
int  comp(const void* a, const void* b);

int binary(int value, int* array, int start, int end) {
	if( start > end )  return -1;
	
	print_range(20, start, end, 4);
	
	if( start == end ) return value == array[start] ? start : -1;

	int mid = (start+end)/2;
	print_middle(20, mid, 4);
	int rez = array[mid] - value;
	if( rez == 0) return mid;
	if( rez < 0) start = mid+1; // look on the right
	else         end   = mid-1; // look on the left
	
	return binary(value, array, start, end);
}


static int call = 0;
int magic_index(int* array, int start, int end) {
	call++; assert(call < 20);// protect against infinite recursion

	print_range(20, start, end, 4);
	if( start > end )       return -1;
	if( array[end] < 0 )    return -1;
	if( array[start] > end) return -1;
	
	

	if( array[start] > start ) start = array[start];
	if( array[end] < end ) end = array[end];

	
	print_range(20, start, end, 4);
	
	if( start == end ) return start == array[start] ? start : -1;

	int mid = (start+end)/2;
	print_middle(20, mid, 4);
	
	
	int rez = array[mid] - mid;
	if( rez == 0) return mid;
	if( rez < 0) {
	  int right = magic_index(array, mid+1, end); // look on the right 
	  if( right != -1) return right;
	  else             return magic_index(array, start, array[mid]); // or maybe on the left	
	} else {
	  int left = magic_index(array, start, mid-1); // look on the left
	  if( left != -1)  return left;
	  else             return magic_index(array, array[mid], end); // or maybe on the right
	}
	
}


int iter_magic(int* array, int len){
	for(int i = 0; i < len; ++i)
		if( array[i] == i ) return i;
	return -1;
}

int main(int argc,char *argv[]) {

	int len = 10, max =+20, min =-20;
	
	if(argc > 1)  len = atoi(argv[1]);
	if(argc > 2)  max = atoi(argv[2]);
	if(argc > 3)  min = atoi(argv[3]);
			
	int *array = malloc(sizeof(int) * len);
	
	srand(time(NULL));
	for(int i = 0;i < len; ++i) array[i] = (rand()%(max-min)) + min; 	

	int value = array[0];
	
	printf("\n\nlooking for %.2i in ", value);  print_ary(array, len);

	qsort( array, len, sizeof(int), &comp);
	printf("sorted array:     "); print_ary(array,len);
	
	binary(value, array, 0, len-1);
	
	printf("\n\nmagic index in    "); print_ary(array,len);
	printf(" bin search found: %i\n", magic_index(array, 0, len-1));
	printf("iter search found: %i\n\n",   iter_magic(array,len));
	
	free(array);
	exit(0);

}


void print_ary(int *ary, int len) {
	printf("[ ");
	for(int i = 0; i < len; ++i) {
		if( ary[i] >= 0 ) printf(" ");
	  printf("%.2i ", ary[i]);
	}
	printf(" ]\n");
}


void print_range(int offset, int start, int end, int width){
	char* buffer = malloc(sizeof(char)*(width+1));
	buffer[width] = '\0';
	
	
	for(int i = 0; i < offset; ++i) printf(" ");
	
	for(int j = 0; j < width; ++j) buffer[j] = ' ';
	for(int i = 0; i < start; ++i) printf("%s", buffer);
	
	
	for(int j = 0; j < width; ++j) buffer[j] = '|';
	buffer[0] = '-';
	buffer[width-1] = '-';
	for(int i = 0; i < 1+end-start; ++i) printf("%s", buffer);
	printf("\n");
	free(buffer);
}


void print_middle(int offset, int mid, int width){
	char* buffer = malloc(sizeof(char)*(width+1));
	buffer[width] = '\0';
	
	
	for(int i = 0; i < offset; ++i) printf(" ");
	
	for(int j = 0; j < width; ++j) buffer[j] = ' ';
	for(int i = 0; i < mid; ++i) printf("%s", buffer);
	
	
	for(int j = 0; j < width; ++j) buffer[j] = '*';
	buffer[0] = ' ';
	buffer[width-1] = ' ';
	printf("%s\n", buffer);
	free(buffer);
}


int comp(const void* a, const void* b) {
	return *((int*)a) - *((int*)b);
}


