

#ifndef QUIX_DENSITY_MATRIX
  #define QUIX_DENSITY_MATRIX


  #include "quix/quix.h"


  typedef 
    struct dmatrix_proto
    {
      int n;
      double** re;
      double** im;
    } 
    dmatrix;


  typedef void (*density_diag)  (int i);
  typedef void (*density_block) (int i, int j);


  EXT   dmatrix*                dmatrix_new         (int);
  EXT   void                    dmatrix_free        (dmatrix*);

  EXT   void                    dmatrix_diag_loop   (int, density_diag);         
  EXT   void                    dmatrix_loop        (int, density_block);   

  EXT   dmatrix*                dmatrix_copy        (dmatrix*);
  EXT   void                    dmatrix_copyto      (dmatrix*, dmatrix*);

  EXT   void                    dmatrix_to_zero     (dmatrix*);
  EXT   void                    dmatrix_to_Id       (dmatrix*);
  EXT   void                    dmatrix_transpose   (dmatrix*);
  EXT   void                    dmatrix_conjugate   (dmatrix*);

  EXT   double*                 dmatrix_diagonal    (dmatrix*);    
  EXT   double                  dmatrix_trace       (dmatrix*);
  EXT   void                    dmatrix_renorm      (dmatrix*);
  EXT   void                    dmatrix_sym         (dmatrix*);
  EXT   double                  dmatrix_avg_phot    (dmatrix*);
  EXT   double                  dmatrix_purity      (dmatrix*);
  EXT   double                  dmatrix_neg         (dmatrix*);
  EXT   dmatrix*                dmatrix_prod_alloc  (dmatrix*, dmatrix*);
  EXT   void                    dmatrix_prod        (dmatrix*, dmatrix*, dmatrix*);

  EXT   double                  dmatrix_dist_max    (dmatrix*, dmatrix*);
  EXT   double                  dmatrix_dist_frob   (dmatrix*, dmatrix*);


#endif


