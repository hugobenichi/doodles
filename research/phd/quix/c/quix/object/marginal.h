#ifndef QUIX_MARGINAL
  #define QUIX_MARGINAL


  #include "quix/object/histogram.h"


  typedef 
    struct proto_marginal
    {
      int                   samples;
      double*               quad;
      double*               phase;
    } 
    marginal;


  EXT   marginal*       marginal_new               (int);
  EXT   marginal*       marginal_copy               (int, double*, double*);
  EXT   void            marginal_free               (marginal*);
  EXT   histogram*      marginal_to_raw_histogram   (marginal*, grid*);

  EXT   double          marginal_draw_gaussian      (double);
  EXT   void            marginal_phase_noise        (marginal*, double);
  EXT   void            marginal_quad_noise         (marginal*, double);
  EXT   marginal*       marginal_teleportation      (marginal*, double);
  EXT   marginal*       marginal_loss               (marginal*, double);

  typedef double        (*probability)(double,double);

  EXT   marginal*       marginal_mc_bootstrap       (marginal*, int);
  EXT   void            marginal_mc_proba           (marginal*, probability, int, double, double);
  EXT   void            marginal_mc_proba_to_quad   (double*, probability, int, double, double);
  EXT   marginal*       marginal_mc_histogram       (histogram*, int, double);


#endif


