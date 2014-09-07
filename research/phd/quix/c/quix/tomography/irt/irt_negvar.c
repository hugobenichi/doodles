

#include "quix/tomography/irt/irt_mc_variance.h"


EXT negvar_wk* irt_negvar_new
  (int n)
{

  negvar_wk* m   = quix_alloc(negvar_wk,1);
  m-> n = n;
  m-> dispersion = double_alloc(n);
  m-> negativity = double_alloc(n);
  m-> variance   = double_alloc(n);

  return m;

}


EXT void irt_negvar_free          
  (negvar_wk* m)
{
  
  if (m)
  {
    if ( m-> dispersion && m-> negativity && m-> variance)
    {
      free( m-> dispersion );
      free( m-> negativity );
      free( m-> variance );
      m-> dispersion = NULL;
      m-> negativity = NULL;
      m-> variance   = NULL;
    } else already_free("IRT_NEGVAR");
  } else already_null("IRT_NEGVAR");

}


