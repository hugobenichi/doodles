#ifndef DYNAMIC_HEAP
  #define DYNAMIC_HEAP    

  #include "heap_api.h"

  /* internal interface */

  struct 
    dheap
    {
      double*   vals;
      int       heap_size;
      int       vals_size;
    };

  #define v_at(heap,index)      ((heap)->vals[(index)])
  #define p_of(heap,index)      ((heap)->vals[(index-1)/2])
  #define l_of(heap,index)      ((heap)->vals[2*(index)+1])
  #define r_of(heap,index)      ((heap)->vals[2*(index)+2])

  void    aryheap_maintain      (double*, int, int);
  void    aryheap_sort          (double*, int);

#endif
