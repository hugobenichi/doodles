#ifndef DYNAMIC_HEAP_API
  #define DYNAMIC_HEAP_API    


  /* public API */


  typedef 
    struct dheap
    dheap;


  /* methods */  

  void      dheap_new         (dheap**, int);
  void      dheap_free        (dheap*);
  void      dheap_in          (dheap*, double val);
  void      dheap_sort        (dheap*);
  void      dheap_arrange     (dheap*);
  void      dheap_iterate     (dheap*, void block(double, void*), void* );

  typedef 
    struct heap_iterator 
    heap_iterator;


  #define   heap_iterator_make
  #define   heap_iterator_next
  #define   heap_iterator_next?
  #define   heap_iterator_value


  #include "dheap.h"
  #include "dheap_iter.h"


#endif
