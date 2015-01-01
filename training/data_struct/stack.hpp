/*
 *	Simple integer stack class for learning C++
 *  copyright hugo benichi 2012
 *  mail hugo[dot]benichi[at]m4x[dot].org
 *
 */
 
#ifndef DATASTRUCT_CPP_STACK_H 
#define DATASTRUCT_CPP_STACK_H 

class List;
 
class Stack { 
	public:
		Stack();                  // create a new empty stack
	 ~Stack();									// delete the stack
		
		void 	push(int val);			// add val on top of the stack
		int 	pop();              // return and remove value from top of the stack
		int	  peek(); 						// return and does not remove value from top of the stack
		int   is_empty();		      // return 0 if not empty; 1 if empty
		int   not_empty();        // return 1 if not emptyl 0 if empty
		
  //private:
    List  values;		
};

#endif
