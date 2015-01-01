#ifndef DYNAMIC_HEAP
  #define DYNAMIC_HEAP    

  /* public API */

  struct 
    dheap
    {
      double**  vals;
      int       heap_size;
      int       depth;
      int       depth_max;
    }; 


  void    dheap_augment     (dheap*);
  void    dheap_maintain    (dheap*);

  #define dpth_of(i)       (floor(log2(i+1)))
  #define rind_of(i)       (  )

  #define ip_of(h,i)       (dpth_of(h,i)-1)][i/2
  #define il_of(h,i)  
  #define ir_of(h,i)

  #define p_of(h,i)
  #define l_of(h,i)  
  #define r_of(h,i)

#endif
