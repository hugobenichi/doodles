#include "wigner_c.h"


EXT wigner_c* wigner_comp_new
  (grid* g)
{

  int i, nx = g->x->n_val, ny = g->y->n_val;

  wigner_c* w = quix_object_alloc(wigner_c);
  w->gd = grid_copy(g);
  w->re  = quix_alloc(double*, nx);
  w->im  = quix_alloc(double*, nx);

  loop(i,nx){
    (w->re)[i] = double_alloc(ny);
    (w->im)[i] = double_alloc(ny);
  }

  return w;

}


EXT void  wigner_comp_free
  (wigner_c* w)
{

  if ( quix_object_free(w) ) 
  {
    if (w->gd && w->gd->x)
    {
      int i, nx = w-> gd-> x-> n_val;    // !!!! use grid object which can be already free 
      if (w-> re && w-> im) 
      {
        loop(i,nx) {
          free( w->re[i]);
          free( w->im[i]);
        }
      }
    } else already_null("Wigner_Comp::Grid");
    free( w->re );
    free( w->im );
    grid_free( w->gd);
    w->re = NULL;
    w->im = NULL;
    w->gd = NULL;
    free(w);
  } else already_free("Wigner_Comp");

}


EXT wigner_c* wigner_comp_convert
  (wigner* w)
{

  int i, j, nx = w->gd->x->n_val, ny = w->gd->y->n_val; 

  wigner_c* c = wigner_comp_new( w->gd );

  loop(i,nx)
    loop(j,ny) {
      c->re[i][j] = w->val[i][j];
      c->im[i][j] = 0;
    }

  return c;

}


EXT wigner** wigner_comp_downconvert
  (wigner_c* w)
{

  wigner** u = quix_alloc(wigner*,2);

  u[0] = wigner_new(w->gd);
  u[1] = wigner_new(w->gd);  

  int i, j, nx = w->gd->x->n_val, ny = w->gd->y->n_val; 

  loop(i,nx)
    loop(j,ny) {
      u[0]->val[i][j] = w->re[i][j];
      u[1]->val[i][j] = w->im[i][j];
    }

  return u;

}


EXT wigner_c* wigner_comp_generate
  (grid* g, wigner_sample re_gen, wigner_sample im_gen)
{

  int i,j, nx = g->x->n_val, ny = g->y->n_val; 

  double *x = g->x->val,
         *y = g->y->val;

  wigner_c* w = wigner_comp_new(g);

  loop(i, nx)
    loop(j, ny) {
      (w->re)[i][j] = re_gen( x[i], y[j] );
      (w->im)[i][j] = im_gen( x[i], y[j] );
    }

  return w;

}



