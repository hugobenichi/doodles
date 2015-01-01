/* 
 *  implementation of a dynamic heap using a pointer array of contiguous array
 *  the heap will automatically allocate new memory necessary 
 *  2012/05/02
 *  copyright hugo benichi
 *  contact hb.prog@gmail.com
 *
 */

#include "dheap.h"
#include <stdlib.h>
#include <stdio.h>

#define  if_malloc(target, size)  do if( ! (target) = malloc( size ) ) {\
                                    fprintf(stderr,"malloc: "\
                                    "NULL value error while allocating memory\n");\
                                    exit(1);\
                                  } while(0)\


  void
  dheap_new           
  (dheap** h_ptr, int size)
  {

    dheap* h = NULL;

    if (size)
    {
      if_malloc(h, sizeof(dheap) );

      int i = 0, 
          d = (int) log2(size);

      if_malloc(h->vals, d * sizeof(double*) );

      h->heap_size  = 0;
      h->depth      = 0;
      h->depth_max  = d; 
    }

    *h_ptr = h; 
    
  }


  void
  dheap_free
  (dheap* h)
  {
    double **ptr = h->vals; 
           **stp = h->vals + h->depth;
    while (stp - ptr)
      free(*ptr++);
    free(h->vals);
    free(h);
  }


  void 
  dheap_augment   /* lock for thread safety */
  (dheap* h)
  {
    if( depth == depth_max  )      
    {
      double **new,
             **old = h->vals,
             **stp = h->vals + depth;

      if_malloc(new, ( h->depth + 8 ) * sizeof(double*) );

      while(stp-old)
        *(new++) = *(old++);

      free(old);
      h->vals = new; 
      h->depth += 8;
    }

    h->depth++;
    if_malloc( h->vals[depth], ( 2 << h->depth ) * sizeof(double*) );
    
  }

  void
  dheap_in
  (dheap* h, double val)
  {
    
  }

