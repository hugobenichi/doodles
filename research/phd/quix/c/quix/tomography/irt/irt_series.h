

#ifndef QUIX_SIRT
  #define QUIX_SIRT


  #include "quix/tomography/series.h"
  #include "quix/object/wigner.h"
  #include "quix/object/histogram.h"
  #include "quix/object/marginal.h"


  EXT   void        sirt_calculate              (series*, marginal*);
  EXT   double      sirt_calculate_neg          (series*, marginal*);
  EXT   double      sirt_calc_neg_var           (series*, marginal*);
  EXT   double*     sirt_calc_neg_var_conv      (series*, marginal*, double*);


#endif


