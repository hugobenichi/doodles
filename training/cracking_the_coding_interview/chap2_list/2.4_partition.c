#include <stdlib.h>
#include <stdio.h>
#include "list.h"

llist* partition(llist* l, int x) {

	llist *before = NULL, *after = NULL, *before_end;

	while( l ) {
		llist *next = l->next;
		if( l-> val < x) {
		printf("found before %i\n", l->val);
			l-> next = before;
			if( !before )
				before_end = l;
			before = l;
		} else {
		printf("found after  %i\n", l->val);
			l->next = after;
			after = l;
		}
		l = next;
	}
	before_end-> next = after;
	
	return before;
	
}


int main(int argc, char* argv[]) {

	int ary[] = {4,5,1,6,0,2,3};

	llist* l = ary2llist( ary , 7 );

	llistprint( l );
	
	int k = 2;
	if ( argc > 1) {
		k = atoi(argv[1]);
	}
	
	l = partition(l, k);
	
	llistprint( l );

	llist_free(l);

	exit(0);

}
