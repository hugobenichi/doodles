

#ifndef QUIX_RANGE
  #define QUIX_RANGE


  #include "quix/quix.h"


  typedef 
    struct proto_range
    {
      double      resolution;
      double      amplitude;
      int         n_val;
      double*     val;
    } 
    range;


  EXT   range*    range_new           (int);
  EXT   range*    range_copy          (range*);
  EXT   range*    range_single        (int, double);
  EXT   range*    range_dual          (int, double);
  EXT   void      range_free          (range*);
  EXT   void      range_symmetrize    (range*);


#endif


