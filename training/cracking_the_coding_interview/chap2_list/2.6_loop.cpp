#include <data_struct/list.hpp>

#include <stdio.h>
#include <stdlib.h>


List* loop_head(List* l){
	if( l == 0 && l->next == 0) return 0;
	List *slow = l, *fast = l;
	do {
		slow = slow->next;
		if(fast->next == 0) return 0;
		fast = fast->next;
		if(fast->next == 0) return 0;
		fast = fast->next;
	} while( slow != fast );

	printf("found loop -> %i\n", slow->val);
	
	slow = l;
	while(slow != fast) {
		slow = slow->next;
		fast = fast->next;
	}
	
	return slow;
}


int main(int argc, char* argv[]){

	if(argc > 2) {

		int len = atoi(argv[1]);
		int p   = atoi(argv[2]);

		List* list = new List(0);
		List* y = list;

		for(int i = 1; i < len; ++i)
			list->append(i);
			
		list->print();

		for(int i = 0; i < p; ++i)
			y = y->next;
			
		printf("looping at %i\n", y->val);
			
		list->last()->next = y;
		
		List* head = loop_head(list);
		
		printf("found looping at %i\n", head->val);
	}

}
