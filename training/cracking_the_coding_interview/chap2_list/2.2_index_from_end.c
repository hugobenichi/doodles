#include <stdlib.h>
#include <stdio.h>
#include "list.h"

llist* index_from_end(llist* l, int k) {
	int length = size(l);
	
	length--;
	while( length > k && l-> next ) {
		length--;
		l = l-> next;
	}
	return l;
}


llist* index_from_end_iter(llist* l, int k) {
	llist *head = l, *tail = l;
	
	int i;
	for( i = k; i > 0 && tail; i--)
		tail = tail->next;

	if(tail) {
		while(tail->next) {
			tail = tail->next;
			head = head-> next;
		}
	}
	
	return head;
}


int main(int argc, char* argv[]) {

	int ary[] = {0,1,2,3,4,5,6};

	llist* l = ary2llist( ary , 7 );
	
	int k = 2;
	if ( argc > 1) {
		k = atoi(argv[1]);
	}
	
	printf( "%i -> %i\n", k, index_from_end(l, k)->val  );
	printf( "%i -> %i\n", k, index_from_end_iter(l, k)->val  );

	llist_free(l);

	exit(0);

}
