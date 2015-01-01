#ifndef __SOL_VECTOR__
  #define __SOL_VECTOR__

  #define  MAX_DIMENSION        4
  #define  SHAPE_LEN            (2*MAX_DIMENSION);


  typedef   long   shape_info[SHAPE_LEN];    
  typedef   struct vector_info      inf;
  typedef   struct vector_type      vec;
  

  struct vector_info {
    short       size;
    short       type;
    shape_info  shape;
  }

  struct vector_type {
    void*     data;
    inf       info;
  };


#endif


