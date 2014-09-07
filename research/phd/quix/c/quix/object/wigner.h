#ifndef QUIX_WIGNER
  #define QUIX_WIGNER


  #include "quix/grid/grid.h"


  typedef 
    struct proto_wigner
    {
      grid*     gd;
      double**  val;
    } 
    wigner;


  typedef double (*wigner_sample)(double x,double y);
  typedef void (*wigner_block)(int i, int j);


  EXT   wigner*                 wigner_new              (grid*);
  EXT   void                    wigner_free             (wigner*);

  EXT   void                    wigner_loop             (wigner_block, int, int);
  EXT   wigner*                 wigner_generate         (grid*, wigner_sample);
  EXT   void                    wigner_to_zero          (wigner*);

  EXT   double                  wigner_norm             (wigner*);
  EXT   void                    wigner_renorm           (wigner*);
  EXT   double                  wigner_purity           (wigner*);
  EXT   double                  wigner_ripple           (wigner*, double);
  EXT   double                  wigner_max              (wigner*);
  EXT   double                  wigner_min              (wigner*);
  EXT   double                  wigner_overlap          (wigner*, wigner*);
  EXT   double                  wigner_dist_over        (wigner*, wigner*);
  EXT   double                  wigner_dist_l2          (wigner*, wigner*);

  EXT   double*                 wigner_x_marg           (wigner*);
  EXT   double*                 wigner_p_marg           (wigner*);

  EXT   wigner*                 wigner_linear           (wigner**, double*, int);

  EXT   wigner*                 wigner_convolate        (wigner*, wigner*);
  EXT   wigner*                 wigner_gaussian_conv    (wigner*, double);

  //EXT   void                    wigner_teleport         (wigner*, double);
  EXT   void                    wigner_loss             (wigner*, double);

  EXT   wigner*                 wigner_teleport         (wigner*, double, grid*); 
  EXT   double                  wigner_teleport_neg     (wigner*, double);
 

#endif


