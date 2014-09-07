

#include "quix/tomography/maxlik/maxlik_dmatrix.h"


EXT maxlik_dmatrix_wk* maxlik_dmatrix_new
  (void)
{

  return  quix_object_alloc(maxlik_dmatrix_wk);

}


EXT void maxlik_dmatrix_free
  (maxlik_dmatrix_wk* wk)
{

  if ( quix_object_free(wk))
  {
    free_if( wk-> distance ) 
    wk-> distance = NULL;
    wk-> data     = NULL;
    free( wk );
  }

}


EXT dmatrix* maxlik_m2dmatrix
(maxlik_dmatrix_wk* wk)
{

  wk-> distance  = double_alloc(wk-> iterations);
  dmatrix* state = dmatrix_new(wk-> photons);
  dmatrix_to_Id(state);  

  if(wk->n_thread)
    maxlik_m2dmat_multi(wk, state, wk->n_thread);
  else
    maxlik_m2dmat_single(wk, state);

  return state;

}


EXT dmatrix* maxlik_h2dmatrix
(maxlik_dmatrix_wk* wk)
{

  wk-> distance  = double_alloc(wk-> iterations);
  dmatrix* state = dmatrix_new(wk-> photons);
  dmatrix_to_Id(state);  

  if(wk->n_thread)
    maxlik_h2dmat_multi(wk, state, wk->n_thread);
  else
    maxlik_h2dmat_single(wk, state);

  return state;

}


