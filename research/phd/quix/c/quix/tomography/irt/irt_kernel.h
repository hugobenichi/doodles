

#ifndef QUIX_KIRT
  #define QUIX_KIRT


  #include "quix/object/wigner.h"
  #include "quix/object/histogram.h"
  #include "quix/object/marginal.h"


  EXT   wigner*           kirt_marginal           (marginal*, double, grid*, int);
  EXT   void              kirt_m2rad              (marginal*, double, grid*, wigner*, int);
  EXT   void              kirt_m2xy               (marginal*, double, grid*, wigner*, int);
  EXT   void              kirt_m2rad_single       (marginal*, double, grid*, wigner*);
  EXT   void              kirt_m2xy_single        (marginal*, double, grid*, wigner*);
  EXT   void              kirt_m2neg              (marginal*, double, double*);
  EXT   void              kirt_m2neg_var          (marginal*, double, double*, double*);

  EXT   wigner*           kirt_histogram          (histogram*, double, grid*, int);
  EXT   void              kirt_h2rad              (histogram*, double, grid*, wigner*, int);
  EXT   void              kirt_h2xy               (histogram*, double, grid*, wigner*, int);
  EXT   void              kirt_h2rad_single       (histogram*, double, grid*, wigner*);
  EXT   void              kirt_h2xy_single        (histogram*, double, grid*, wigner*);
  EXT   void              kirt_h2neg              (histogram*, double, double*);
  EXT   void              kirt_h2neg_var          (histogram*, double, double*, double*);


#endif


