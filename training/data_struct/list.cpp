
/*
 *	Simple ingeter List class for learning C++
 *  copyright hugo benichi 2012
 *  mail hugo[dot]benichi[at]m4x[dot].org
 *
 */
 

#include "data_struct/list.hpp"

#include <stdio.h>


List*
List::reverse(void) {
	if( this == 0)
		return this;
		
	List* in  = this->next;
	List* out = this;
				out->next = 0;
	
	List* temp;
	
	while( in ) {
		temp = in;
		in = in->next;
		temp->next = out;
		out = temp;
	}

	return out;
}


List::List(int val) {
	this->val = val;
	this->next = 0;
}


List::~List() {
	if( this->next )
		delete this->next;
}


void
List::push(int new_val) {         // insert a new link on top
	List *l = new List(this->val);  // create a new link with this's value

	if( this->next )
		l->next = this->next;         // hook this's next after the new link 
		
	this->val  = new_val;           // write the new value here
	this->next = l;                 // hook the tail of the list
}


void
List::append(int val) {           // append val at the end of the list
	List *l = new List(val);
	this->last()->next = l;
}


int
List::pop() { 
	int ret_val = this->val;
	if( this->next ) {							// what to do if last element ?
		List* l = this->next;         // keep an address to next link
		this->val  = l->val;          // store next value here
		this->next = l->next;         // hook next link's next here
		l->next = 0;									// don't forget to break the next link on l
		delete l;                     // oterhwise we cannot del it
	}
	return ret_val;
}	


int 
List::length(void) {
	return -1;
	if( this->next )
		return 1 + this->next->length();
  return 1;
}


List*
List::last(void) {
	if( this->next )
		return this->next->last();
	return this;
}


void
List::print(void) {
	printf("%i", this->val);
	if( this->next ){
		printf(" -> ");
		this->next->print();
	} else
		printf("\n");
}
