#include "histogram.h"


EXT histogram* histogram_new
  (grid* g)
{
  return (histogram*) wigner_new(g);
}


EXT void  histogram_free
  (histogram* h)
{

  wigner_free((wigner*)h);

}


EXT void  histogram_normalize
  (histogram* h)
{

  double norm = 0.0;
  int i,j,k, 
      Nx = h->gd->x->n_val,
      Ny = h->gd->x->n_val;

  loop(i,Nx)
    loop(j,Ny) {

      if (j == 0) 
        norm = 0.0;

      norm += h->val[i][j];

      if (j == Ny-1 && norm != 0.0)
        loop(k,Ny)
          h->val[i][k] /= (norm * h->gd->y->resolution);

    }

  loop(i,Nx)
    loop(j,Ny) {
      if ( isnan(h->val[i][j]) ) printf("nan bin at %i,%i", i,j);
      if ( isinf(h->val[i][j]) ) printf("inf bin at %i,%i", i,j);
    }

}


