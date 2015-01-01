#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

#define VECTOR_INIT_CAP = 16;

int vector_new(vector** pointer_to_address) {

	vector* vec = malloc(sizeof(vector));

	vec->data = malloc( VECTOR_INIT_CAP );

	vec->capacity = VECTOR_INIT_CAP;
	vec->len = 0;

	*pointer_to_address = vec;
}

int vector_grow(vector* vec) {
	void* bigger_data = realloc(vec->data, 2*(vec->capacity));
	vec->data = bigger_data;
	return 0;
}

int vector_add(vector* vec, void* object){
	int ret_code = -1;

	if (!vec) goto exit;
	
	if (vec->len + 1 == vec-> capacity)
		if ( vector_grow(vec) )
			goto exit;
	
	vec->data[vec->length] = object;
	vec->data++;
	ret_code = -1

exit:
	return ret_code;
}

void* vector_pop(vector* vec) {
	if (!vec || vec->length == 0 ) return NULL;
	return vec->data[vec->length--];

}

int vector_push(vector* vec, void* object) {
	
	
}
