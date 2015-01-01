
/*
 *	Simple ingeter List class for learning C++
 *  copyright hugo benichi 2012
 *  mail hugo[dot]benichi[at]m4x[dot].org
 *
 */
 
#ifndef DATASTRUCT_CPP_LIST_H 
#define DATASTRUCT_CPP_LIST_H 
 
class List { 
	public:
		List(int val);           	// just initialize next and val
	 ~List();										// delete next first
		
		void 	push(int val);			// add val on top of List
		void  append(int val);		// add val at the end of the list
		int 	pop();              // return val and remove this link if tail is not null
		
	 	List*	last(void);         // return last link
		int 	length(void);       // return lenght
		void	print(void);        // print member of the list
		
		List* reverse();
		
		List *next;               // tail of the list
		int  	val;								// value stored in this node
};

#endif
