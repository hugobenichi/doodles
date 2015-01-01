#include <stdlib.h>
#include <stdio.h>
#include "list.h"


int compare(llist* a, llist* b) {
	while( a && b ) {
		if( a->val != b->val )
			return 0;
		a = a->next;		
		b = b->next;
	}
	return !(a || b);
}


int ispal_iter(llist* l) {
	llist *p_fast = l, *p_slow = l;
	llist *stack = NULL;
	
	while( p_fast ) {
		int val = p_slow->val;
	printf("%i\n",val);
		p_slow = p_slow -> next;
		p_fast = p_fast -> next;
		if( p_fast ) {
		  llist* head = llist_new(val);
		  head->next = stack;
			stack = head;
			p_fast = p_fast -> next;
		}
	}
	int rez = compare(p_slow, stack);
	llist_free(stack);
	
	return rez;
}


int main(int argc, char* argv[]) {

	int ary[] = {4,5,1,1,5,4};
	
	llist* l = ary2llist( ary , 6 );
	
	printf( "%i\n", ispal_iter(l) );

	llist_free(l);

	exit(0);

}
