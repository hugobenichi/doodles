#ifndef QUIX_HERMITE
  #define QUIX_HERMITE

  #include "quix/quix.h"
  #include "quix/grid/range.h"


  typedef 
    struct proto_hermite
    {
      int          N;
      range*       rg;
      double**       polynomials;
      double**       harmonics;
    } 
    hermite_workspace;

  typedef
    hermite_workspace
    hermite_wk;


  EXT   hermite_wk*             hermite_new         (int, range*);
  EXT   void                    hermite_free        (hermite_wk*);
  EXT   void                    hermite_calculate   (hermite_wk*);


#endif

