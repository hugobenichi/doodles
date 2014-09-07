

#ifndef QUIX_LAGUERRE
  #define QUIX_LAGUERRE

  #include "quix/quix.h"
  #include "quix/grid/range.h"


  typedef 
    struct laguerre_wk_proto
    {
      int          N;
      range*       rg;
      double***    polynomials;
    } 
    laguerre_workspace;

  typedef
    laguerre_workspace 
    laguerre_wk;

  EXT   laguerre_wk*            laguerre_new          (int, range*);
  EXT   void                    laguerre_free         (laguerre_wk*);
  EXT   void                    laguerre_calculate    (laguerre_wk*);


#endif


