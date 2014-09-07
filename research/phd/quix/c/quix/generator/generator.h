#ifndef QUIX_GENERATOR
  #define QUIX_GENERATOR


  #include "quix/object/wigner.h"
  #include "quix/object/dmatrix.h"
  #include "quix/object/histogram.h"
  #include "quix/object/marginal.h"


  typedef 
    struct proto_generator 
    generator;


  typedef wigner*     (*wigner_generator)     (generator*, grid*);
  typedef dmatrix*    (*dmatrix_generator)    (generator*, int);   
  typedef histogram*  (*histo_generator)      (generator*, grid*, range*);  
  typedef marginal*   (*marginal_generator)   (generator*, int, double);  
  typedef void        (*quadrature_generator) (generator*, double*, int, double);


  struct proto_generator {
    double*                 parameters;
    wigner_generator        wig;       
    dmatrix_generator       dmat;
    histo_generator         hist;
    marginal_generator      marg;
    quadrature_generator    quad;
  };


  EXT   generator*      generator_new               (int);
  EXT   void            generator_free              (generator*);

  EXT   wigner*         generator_make_wigner       (generator*, grid*);
  EXT   dmatrix*        generator_make_dmatrix      (generator*, int);
  EXT   histogram*      generator_make_histogram    (generator*, grid*, range*);
  EXT   marginal*       generator_make_marginal     (generator*, int, double);


  EXT   generator*      generator_vacuum            ();
  EXT   generator*      generator_thermal           (double);
  EXT   generator*      generator_coherent          (double, double);
  EXT   generator*      generator_squeezing         (double);
  EXT   generator*      generator_one               ();
  EXT   generator*      generator_cat               (double, double, double);
  EXT   generator*      generator_sqzd_photon       (double);


  EXT   generator*      generator_vac_tele          (double);
  EXT   generator*      generator_thm_tele          (double,double);
  EXT   generator*      generator_coh_tele          (double, double, double);
  EXT   generator*      generator_sqz_tele          (double, double);


  EXT   generator*      generator_coh_loss_tele     (double, double, double, double);
  EXT   generator*      generator_sqz_loss_tele     (double, double, double);
  EXT   generator*      generator_one_loss          (double);


#endif


