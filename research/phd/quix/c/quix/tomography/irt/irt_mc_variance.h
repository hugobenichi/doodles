

#ifndef QUIX_IRT_MC_VAR
  #define QUIX_IRT_MC_VAR


  #include "quix/object/wigner.h"
  #include "quix/object/histogram.h"
  #include "quix/object/marginal.h"
  #include "quix/generator/generator.h"
  #include "quix/tomography/series.h"


  typedef marginal*  (*get_marginal)    (void);
  typedef histogram* (*get_histogram)   (void);

  typedef 
    struct proto_neg_var 
    {
      int       n;
      double*   dispersion; 
      double*   negativity;
      double*   variance;
    } negativity_variance;

  typedef
    negativity_variance
    negvar_wk;

  EXT   negvar_wk*      irt_negvar_new            (int);
  EXT   void            irt_negvar_free           (negvar_wk*);


  EXT   double*         kirt_mc_h2neg_var         (int, double, get_histogram);///!not finished!
  EXT   double*         kirt_mc_m2neg_var         (int, double, get_marginal);
  EXT   double*         kirt_mc_g2neg_var         (int, double, generator*, int, double);
  EXT   negvar_wk*      kirt_mc_m2negvar_wk       (int, double, get_marginal);
  EXT   negvar_wk*      kirt_mc_g2negvar_wk       (int, double, generator*, int, double);


  EXT   double*         sirt_mc_m2neg_var         (int, series*, get_marginal );
  EXT   negvar_wk*      sirt_mc_m2negvar_wk       (int, series*, get_marginal );


/*
  typedef 
    struct proto_sirt_mc_wk
    {
      int           siz_ang;                    
      int           siz_rad;
      double        amp_rad;
      double****    c_re;       //indices are (n,m) <-> (angular,radial)
      double****    c_im;       //indices are (n,m) <-> (angular,radial)
    } 
    sirt_mc_wk;


  EXT   irts_disp*  fri_disp_alloc              (int, int, double);
  EXT   void        fri_disp_free               (irts_disp*); 
  EXT   irts_disp*  fri_calculate_disp          (radon_ft*, marginal*);
  EXT   wigner*     fri_construct_disp          (irts_disp*, grid*);
*/


#endif


