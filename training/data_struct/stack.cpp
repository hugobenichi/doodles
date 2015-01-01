/*
 *	Simple integer stack class for learning C++
 *  copyright hugo benichi 2012
 *  mail hugo[dot]benichi[at]m4x[dot].org
 *
 */
 
#include <data_struct/stack.hpp>
 
#include <stdio.h>

#include <data_struct/list.hpp>


Stack::Stack(){
  this->values = 0;
}

Stack::~Stack(){
	if( this->values != 0)
    delete this->values;
}

/*
 * if List is null create a new list (empty stack case)
 * if List is non-null, create a new list and append current list to it
 * this allows to keep the same order for the list nodes and the stack history
 * usefull for minstack or "by pointer" stack manipulation
 */
void
Stack::push(int val){
	List new_node = new List(val);
	if ( this->values != 0) {
	  new_node->next = this->values;
	  this->values = new_node;
	} else 
		this->values = new_node;
}

/*
 * if List is null return -1 (empty stack case)
 * if List is non-null, break the head element of the list to preserve nodes order
 * usefull for minstack or "by pointer" stack manipulation
 * and also allows to deal with the "1-element list" pop problem
 */
int
Stack::pop(){
	int rez = -1;
  if( this->values != 0) {
    List pop_node = this->values;
  	this->values = pop_node->next;
  	pop_node->next = 0;
  	rez = pop_node->val;
  	delete pop_node
  }
  return rez;
}

int
Stack::peek(){
	int rez = -1;
  if( this->values != 0)
  	rez = this->values->val;
  return rez;
}

int
Stack::is_empty(){
	return this->values == 0;
}


int
Stack::not_empty(){
	return this->values != 0;
}
 
