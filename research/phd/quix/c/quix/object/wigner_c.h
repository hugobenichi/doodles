#ifndef QUIX_wigner_COMP
#define QUIX_WIGNER_COMP


  #include "quix/grid/grid.h"
  #include "quix/object/wigner.h"


  typedef 
    struct proto_wigner_comp
    {
      grid*       gd;
      double**    re;
      double**    im;
    } 
    wigner_c;


  EXT   wigner_c*         wigner_comp_new             (grid*);
  EXT   void              wigner_comp_free            (wigner_c*);
  EXT   wigner**          wigner_comp_downconvert     (wigner_c*);
  EXT   wigner_c*         wigner_comp_convert         (wigner*);

  EXT   wigner_c*         wigner_comp_generate        (grid*, wigner_sample, wigner_sample);

  /*
  EXT double*                   wigner_comp_cut(wigner_c* w);
  EXT double                    wigner_comp_norm(wigner_c* w);
  EXT double                    wigner_comp_purity(wigner_c* w);
  EXT double                    wigner_comp_max(wigner_c* w);
  EXT double                    wigner_comp_min(wigner_c* w);
  EXT double                    wigner_comp_overlap(wigner_c* w1, wigner_c* w2);
  */


#endif
