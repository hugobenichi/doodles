#ifndef HEAP_API
  #define HEAP_API    

  /* public API */

  typedef 
    struct dheap 
    dheap;


  dheap*    dheap_new           (int);
  void      dheap_free          (dheap*);

  dheap*    dheap_from_array    (double*, int);

  int       dheap_size          (dheap*);
  void      dheap_in            (dheap*, double);
  double    dheap_out           (dheap*);

  double    dheap_at            (dheap*, int);
  void      dheap_del_at        (dheap*, int);  
  void      dheap_up_at         (dheap*, int, double);

  void      dheap_iter          (dheap*, void (*block)(double) );

  int       dheap_parent_of     (dheap*, int);
  int       dheap_lchild_of     (dheap*, int);
  int       dheap_rchild_of     (dheap*, int);

  void      dheap_maintain      (dheap*);
  void      dheap_sort          (dheap*);

#endif
