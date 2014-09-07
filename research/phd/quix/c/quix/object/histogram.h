#ifndef QUIX_HISTOGRAM
  #define QUIX_HISTOGRAM


  #include  "quix/grid/grid.h"
  #include  "quix/object/wigner.h"


  typedef 
    struct proto_histogram
    {
      grid*                  gd;
      double**               val;
    } 
    histogram;


  EXT   histogram*           histogram_new            (grid*);
  EXT   void                 histogram_free           (histogram*);
  EXT   void                 histogram_normalize      (histogram*);


#endif

