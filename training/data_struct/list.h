#include <stdio.h>
#include <stdlib.h>

  struct linkedlist;

  typedef 
    struct linkedlist 
    {
      int                  val;
      struct linkedlist*   next;
    }
    llist;

  llist*    llist_new   (int x);
  void      llist_free  (llist*);
  int       size        (llist* l);
  llist*    last        (llist* l);
  void      appen       (llist* into, llist* l);
  void      insert      (llist* into, llist* l);
  void      add         (llist* into, int x);
  llist*    head        (llist* l);
  llist*    pop         (llist* l);
  llist*    ary2llist   (int* ary, int size);
  void      llistprint  (llist*);

  llist*    sortfuse    (llist*, int, llist*, int);
  void      sortmerge   (llist*, int depth);
  
 
  
  typedef llist stack;
  	
  //stack* stack_new();
  void stack_free(stack*);
  int stack_peek(stack*);
  int stack_pop(stack*);
  void stack_push(stack*, int);
