#ifndef QUIX_FOCK
  #define QUIX_FOCK


  #include "quix/generator/generator.h"
  #include "quix/object/wigner_c.h"


  typedef 
    struct proto_fock_workspace
    {
      grid*                  gd;
      int                    N;
      wigner_c***            photons;
    } 
    fock_workspace;

  typedef fock_workspace   fock_wk;


  EXT   fock_wk*             fock_new               (grid*, int);
  EXT   void                 fock_free              (fock_wk*);
  EXT   void                 fock_calculate         (fock_wk*);

  EXT   wigner*              fock_wig_from_matrix   (fock_wk*, dmatrix*);
  EXT   dmatrix*             fock_wig_to_matrix     (fock_wk*, wigner*);


#endif

