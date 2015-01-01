/*
 *	Stacks of integer values which allos quick access to the min elements
 *  guarantees O(1) for push, pop and min
 *  question 3.2 of "cracking the coding interview"
 *  copyright hugo benichi 2012
 *  mail hugo[dot]benichi[at]m4x[dot].org
 *
 */
 
#include <chap3_stack_heap/minstack.hpp>
 
#include <stdio.h>

#include <data_struct/minstack.hpp>


Minstack::Minstack(){
  this->values = new Stack();
  this->mins   = new Stack();  // change this to new stack of List pointers
}


Minstack::~Minstack(){
  delete this->values;
  delete this->mins; 
}

/*
 *  this function acts in two steps  
 *    1) first it simply push the new values to the stack
 *    2) then it checks if it needs to push a new minimum value
 *       the second stack:
 *         if second stack empty -> push value
 *         if second stack top is higher -> push value
 */
void
Minstack::push(int val){
	this->values->push(val);
	
	if ( this->mins.is_empty()           // push val to mins stack if min stack empy
	|| ( val < this->min() ) )           //                        or if new minimum
		this->mins->push( this->values );  // store current top list node
		
		
}


/*
 *  this function acts in two steps
 *    1) first it simply pops and saves the current top of the stack
 *    2) then it checks if it needs to remove the top minimum:
 *         if first stack now empty -> pop min
 *         if first stack
 */
int
Minstack::pop(){ // pop is relou to implement
	int rez = -1;
  if( this->values->non_empty() ) {
  	List top_node = this->values;
		rez = this->values->pop();		
  	if( this->mins->peek() == top_node )  // pop from mins if pointing to top_node
  		this->mins->pop();
  }   
  return rez;
}


int
Minstack::min(){
	int rez = -1;
	if ( this->mins->non_empty() )     // need to perform empty test to guard against null list
		rez = this->mins->peek()->val;   // mins is a stack of list node
	return rez;
}


int
Minstack::peek(){
	return this->values->peek();  //perform empty test inside this->values
}


int
Minstack::is_empty(){
	return this->values->is_empty();
}


int
Minstack::not_empty(){
	return this->values->not_empty();
}
