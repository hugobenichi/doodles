#include "data_struct/list.hpp"
#include <stdio.h>
#include <stdlib.h>

List* int2list(int x) {

	List* l = new List( x % 10);
	x /= 10;
	while(x){
		l->append(x%10);
		x /= 10;
	}
	return l;
}

List* ladd(List* a, List* b) {
	
	List* l = 0;
	List* last = 0;
	
	int carry = 0;
	while( a || b ) {
		int sum = carry;
		if( a ) {
			sum += a-> val;
			a = a-> next;	
		}
		if( b ) {
			sum += b-> val;
			b = b-> next;
		}
		carry = sum / 10;
		List* c = new List(sum%10);
		if (l == 0) {
			l = c;
			last = c;
		} else {
			last->next = c;
			last = c;
		}
	}

	return l;
}


int laddrevrec(List* a, List* b, List **c) {
	int sum = a->val + b->val;
	if( a->next ) {
		sum += laddrevrec( a->next, b->next, c );
		(*c)->push(sum%10);
	} else {
		*c = new List(sum%10);
	}
	return sum/10;
}

List* laddrev(List* a, List* b) {

	List *a_end = a, *b_end = b;
	while(a_end && b_end) {
		a_end = a_end->next;
		b_end = b_end->next; 
	}
	
	List *l_short = (a_end == 0) ? a : b; 
	List *l_long  = (a_end == 0) ? b_end : a_end;
	int padding = 0;
	while(l_long) {
		l_short->push(0);
		l_long = l_long -> next;
		padding++;
	}	
	
	List *c = 0;
	
	int carry = laddrevrec(a,b,&c);
	if( carry > 0)
		c->push(carry);
		
	while( padding ){
		l_short->pop();
		padding--;
	}

	return c;
}

int main(int argc, char* argv[]){

	if(argc > 2) {
		List* la = int2list(atoi(argv[1]));
		List* lb = int2list(atoi(argv[2]));
		List* lc = ladd(la,lb);
/*		
		la->print();
		lb->print();
		lc->print();
*/		
		List* lar = la->reverse();
		List* lbr = lb->reverse();
		List* lcr = laddrev(lar,lbr);
		
		lar->print();
		lbr->print();
		lcr->print();
		
		delete lar;
		delete lbr;
		delete lc;
		delete lcr;
	}


}
