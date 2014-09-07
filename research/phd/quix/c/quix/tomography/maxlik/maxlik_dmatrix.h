#ifndef QUIX_MAXLIKE_DMATRIX
  #define QUIX_MAXLIKE_DMATRIX


  #include "quix/polynomial/hermite.h"
  #include "quix/grid/range.h"
  #include "quix/object/dmatrix.h"
  #include "quix/object/histogram.h"
  #include "quix/object/marginal.h"


  /*
    les conventions pour (proj)[i][j] et (state)[i][j] sont inversees

      (PI)(m,n)  <-> (proj)[i][j]

      (rho)(n,m) <-> (state)[i][j]

      (R)(n,m)   <-> (operator)[i][j]

  */

  typedef void (*maxlik_cb)(dmatrix*);


  typedef 
    struct maxlik_dmatrix_wk_proto 
    {
      int             report;
      int             iterations;
      int             photons;
      int             n_thread;
      double*         distance;
      void*           data;
      maxlik_cb       callback;
    } 
    maxlik_dmatrix_wk;


  EXT   maxlik_dmatrix_wk*      maxlik_dmatrix_new        (void);
  EXT   void                    maxlik_dmatrix_free       (maxlik_dmatrix_wk*);

  EXT   dmatrix*                maxlik_m2dmatrix          (maxlik_dmatrix_wk*);
  EXT   void                    maxlik_m2dmat_single      (maxlik_dmatrix_wk*, dmatrix*);
  EXT   void                    maxlik_m2dmat_multi       (maxlik_dmatrix_wk*, dmatrix*, int);

  EXT   dmatrix*                maxlik_h2dmatrix          (maxlik_dmatrix_wk*);
  EXT   void                    maxlik_h2dmat_single      (maxlik_dmatrix_wk*, dmatrix*);
  EXT   void                    maxlik_h2dmat_multi       (maxlik_dmatrix_wk*, dmatrix*, int);

  EXT   void                    maxlik_m2diag             (maxlik_dmatrix_wk*, double*);
  EXT   void                    maxlik_h2diag             (maxlik_dmatrix_wk*, double*);

#endif


