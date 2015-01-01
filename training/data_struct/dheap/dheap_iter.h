

#ifndef DHEAP_ITERATOR
  #define DHEAP_ITERATOR
  

  /* iterator API */


  struct heap_iterator
  {
    double  **depth;
    double   *pointer;  
    double   *stop;
    int       index;
    int       has;
  }

  #ifdef heap_iterator_make
    #undef heap_iterator_make
  #endif

  #ifdef heap_iterator_next
    #undef heap_iterator_next
  #endif

  #ifdef heap_iterator_next?
    #undef heap_iterator_next?
  #endif

  #ifdef heap_iterator_value
    #undef heap_iterator_value
  #endif

  #define\
    heap_iterator_make(hp_pointer)\
    {\
      (hp_pointer)->vals[0],\
      (hp_pointer)->vals[0][0],\
      (hp_pointer)->vals[0][1],\
      (hp_pointer)->heap_size,\
      0
    }

  #define\
    heap_iterator_next(hp_itr)\
    do {\
      (hp_itr).index++;\
      (hp_itr).pointer++;\
      if ( !( (hp_itr).stop - (hp_itr).pointer ) )\
      {\
        (hp_itr).depth++;\
        (hp_itr).pointer = *(hp_itr).depth;\
        (hp_itr).stop    = *(hp_itr).depth + (hp_itr).index + 1;\
      }\
    } while(0)\

  #define\
    heap_iterator_next?(hp_itr)\
    ( (hp_itr).has - (hp_itr).index )  /* check index here */

  #define\
    heap_iterator_value(hp_itr)\
    *(hp_itr).pointer


#endif


