/*
 *	Stacks of integer values which allos quick access to the min elements
 *  guarantees O(1) for push, pop and min
 *  question 3.2 of "cracking the coding interview"
 *  copyright hugo benichi 2012
 *  mail hugo[dot]benichi[at]m4x[dot].org
 *
 */
 
#ifndef DATASTRUCT_CPP_MINSTACK_H 
#define DATASTRUCT_CPP_MINSTACK_H 

class Stack;
 
class Minstack { 
	public:
		Minstack();               // create a new empty stack
	 ~Minstack();								// delete the stack
		
		void 	push(int val);			// add val on top of the stack
		int 	pop();              // return and remove value from top of the stack
		int	  peek(); 						// return and does not remove value from top of the stack
		int   is_empty();		      // return 0 if not empty; 1 if empty
		int   not_empty();        // return 1 if not empty; 0 if empty
		int   min();
		
  //private:
    Stack  values;		        // encapsulated stack of values
    Stack  mins;              // list of minimums
};

#endif
