/* 
 *  implementation of heap using a linear contiguous array
 *  2012/05/02
 *  copyright hugo benichi
 *  contact hb.prog@gmail.com
 *
 */

  #include "heap_lin.h"


  #include <stdlib.h>
  #include <stdio.h>


  #define  if_malloc(target, size)  do if (!( target = malloc( size ) )) {\
                                      fprintf(stderr,"malloc: "\
                                      "NULL value error while allocating memory\n");\
                                      exit(1);\
                                    } while(0)\


  void    
  ary_print             
  (double* ary, int l);

/* allocators */


  dheap*
  dheap_new           
  (int size)
  {
    dheap* h = NULL;

    if (size)
    {
      if_malloc(h, sizeof(dheap) );
      if_malloc(h->vals, size );
      h->heap_size = 0; 
      h->vals_size = size; 
    }

    return h;
    
  }

  dheap*    
  dheap_from_array    
  (double* ary, int l)
  {
    dheap* h = dheap_new(l);
    int i = 0;
    while(l-i)
      dheap_in(h, ary[i++]);

    return h;
  }


/* desallocator */


  void      
  dheap_free          
  (dheap* h)
  {
    free(h->vals);
    free(h);
  }


/* internal interface wrapper to public api */


  int       
  dheap_size          
  (dheap* h)
  {
    return h->heap_size;
  }

  double    
  dheap_at            
  (dheap* h, int i)
  {
    return v_at(h,i);
  }

  int       
  dheap_parent_of     
  (dheap* h, int i)
  {
    return p_of(h,i);    
  }

  int       
  dheap_lchild_of     
  (dheap* h, int i)
  {
    return l_of(h,i);    
  }

  int       
  dheap_rchild_of     
  (dheap* h, int i)
  {
    return r_of(h,i);    
  } 

  void    
  dheap_maintain    
  (dheap* h)
  {
    aryheap_maintain(h->vals, 0, h->heap_size);
  }

  void    
  dheap_sort        
  (dheap* h)
  {
    //aryheap_sort(h->vals,h->heap_size);

    while(h->heap_size)
    {
      double out = dheap_out(h);
      v_at(h,h->heap_size) = out;
    }

  }


/* functions using internal interface */


  void      
  dheap_in            
  (dheap* h, double val)
  {
    if(h->heap_size >= h->vals_size)
      return;

    v_at(h,h->heap_size) = val;
    int i = h->heap_size++;

    while( i && p_of(h,i) < val )
    {
      v_at(h,i) = p_of(h,i);
      p_of(h,i) = val;
      i = (i-1) / 2 ;
    }

  }

  double    
  dheap_out           
  (dheap* h)
  {
    double out = v_at(h,0); 
    v_at(h,0) = v_at(h,h->heap_size-- - 1);
    aryheap_maintain(h->vals, 0, h->heap_size);
    return out;
  }

  void      
  dheap_del_at        
  (dheap* h, int index)
  {
    v_at(h,index) = v_at(h,h->heap_size-- - 1);  
    aryheap_maintain(h->vals, index, h->heap_size);
  }
  
  void    
  dheap_up_at       
  (dheap* h, int index, double new)
  {
    double old = v_at(h,index);
    v_at(h,index) = new;
    if ( old > new )
      aryheap_maintain(h->vals, index, h->heap_size);
    else
      aryheap_maintain(h->vals, index, h->heap_size);
  }

  void    
  dheap_iter        
  (dheap* h, void (*block)(double) )
  {
    int i = 0, l = h->heap_size;
    while(l-i)
      block( v_at(h,i++) );
  } 

  void 
  aryheap_maintain
  (double* ary, int i, int heap_size)
  {

    double  max   = ary[i];
    int     i_max = i, 
            l     = 2*i + 1, 
            r     = 2*i + 2;
    //if (l >= heap_size) return;
    if (l < heap_size && ary[l] > max ) {
      max = ary[l];
      i_max = l;
    }  
    if (r < heap_size && ary[r] > max)
    {
      max = ary[r];  
      i_max = r;
    }
    if ( i_max != i )  
    {
      ary[i_max] = ary[i];
      ary[i] = max;
      aryheap_maintain(ary, i_max, heap_size);
    }

/*
    int     i_max = i, 
            l     = 2*i + 1, 
            r     = 2*i + 2;
    //if (l >= heap_size) return;
    if (l < heap_size && ary[l] > ary[i_max] )    i_max = l;
    if (r < heap_size && ary[r] > ary[i_max] )    i_max = r;
    if ( i_max != i )  
    {
      double swap = ary[i_max];
      ary[i_max] = ary[i];
      ary[i] = swap;
      aryheap_maintain(ary, i_max, heap_size);
    }
*/
  }

  void 
  aryheap_sort
  (double* ary, int heap_size)
  {
/*
    double* end = ary + heap_size - 1;
    while(heap_size)
    {
printf("sorting val %.1lf with %.1lf at %d\n", *ary, *end, heap_size);
      double out = *ary; 
      *ary  = *end;
      *end = out;
      end--;
printf("foo\n");
      aryheap_maintain(ary, 0, --heap_size);
printf("bar\n");
    }
*/
  }

#ifdef DHEAP_MAIN


  void    
  ary_print             
  (double* ary, int l)
  {
    int i = 0; 
    while(l-i)
      printf("%.1lf ", ary[i++]);
    printf("\n");
  }

  void    
  ary_rand             
  (double* ary, int l, double mult)
  {
    int i = 0;
    while(l-i)
      ary[i++] = mult * ( rand() / (RAND_MAX + 1.0) ) ;
    printf("\n");
  }

  int 
  main
  (void)
  {

    #include <time.h>
    srand(time(0));

    #define n 12
    double ary[n] = {-1.};

    ary_rand(ary,n,32.);
    ary_print(ary,n);

    //dheap h = { ary, n, n };

    dheap* h = dheap_from_array( ary, n);
  
    printf("\n");    

    ary_print(h->vals,n);

    printf("\n");   
    
    dheap_sort(h);

    ary_print(h->vals,n);    

    //dheap_free(h);

  }

#endif
