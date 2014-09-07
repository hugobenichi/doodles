#include "quix/object/wigner.h"



#define   wigner_loop(index_i,index_j,stop_i,stop_j)  loop(index_i,stop_i)\
                                                        loop(index_j,stop_j)


EXT wigner* wigner_new
  (grid* g)
{

  int i,
      Nx = g-> x-> n_val,
      Ny = g-> y-> n_val;

  wigner* w = quix_object_alloc(wigner);

  w-> gd  = grid_copy(g);
  w-> val = quix_alloc(double*,Nx);
  alloc_check(w-> val,"wigner_new");

  loop(i,Nx) {
    w-> val[i]  = double_alloc(Ny);
    alloc_check(w-> val[i],"wigner_new");
  }

  return w;

}


EXT void  wigner_free
  (wigner* w)
{
  
  if ( quix_object_free(w) ) 
  {
    if (w->gd && w->gd->x)
    {
      int i, nx = w-> gd-> x-> n_val;    // !!!! use grid object which can be already free 
      if (w-> val) 
      {
        loop(i,nx)
          free( w->val[i]);
        free( w->val);
        w->val = NULL;
      } else already_null("Wigner::Val");
      grid_free(w->gd);
      w->gd  = NULL;      
    } else already_null("Wigner::Grid");  
    free(w);
  } else already_free("Wigner");

}


EXT wigner* wigner_generate
  (grid* g, wigner_sample gen)
{

  int i, j,
      Nx = g-> x-> n_val,
      Ny = g-> y-> n_val;

  wigner* w = wigner_new(g);
  double *x = g-> x-> val,
         *y = g-> y-> val;

  wigner_loop(i,j,Nx,Ny)
    w-> val[i][j] = gen( x[i], y[j]);

  return w;

}


EXT void wigner_to_zero
  (wigner* w)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  wigner_loop(i,j,Nx,Ny)
    w-> val[i][j] = 0.0;

}


EXT double wigner_norm
  (wigner* w)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double norm = 0.0;

  if_cartesian(w-> gd)
    wigner_loop(i,j,Nx,Ny)
      norm += w-> val[i][j];

  else
    wigner_loop(i,j,Nx,Ny)
      norm += w-> val[i][j] * w-> gd-> x-> val[i];

  return norm*grid_cell(w->gd);

}


EXT void wigner_renorm
  (wigner* w)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double norm = 1.0 / wigner_norm(w);

  wigner_loop(i,j,Nx,Ny)
    w-> val[i][j] *= norm;

}


EXT double wigner_purity
  (wigner* w)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double purity = 0.0;

  if_cartesian(w-> gd)
    wigner_loop(i,j,Nx,Ny)
      purity += w-> val[i][j] * w-> val[i][j];
  
  else
    wigner_loop(i,j,Nx,Ny)
      purity += w-> val[i][j] * w-> val[i][j] * w-> gd-> x-> val[i];

  return 2*PI * purity * grid_cell(w-> gd);

}


// a quoi sert cette function ??? 
EXT double wigner_ripple
  (wigner* w, double radius)
{
  
  int i, j, n = 0,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double disp = 0.0,
         avrg = 0.0,
         rad2 = radius*radius;

  if_cartesian(w-> gd)
    wigner_loop(i,j,Nx,Ny)
      if ( (w->gd->x->val[i]) >= radius) {
        disp += (w->val)[i][j]*(w->val)[i][j];
        avrg += (w->val)[i][j];
        n++;
      }

  else
    wigner_loop(i,j,Nx,Ny)
      if ( 
          (w->gd->x->val[i])*(w->gd->x->val[i]) 
        + (w->gd->y->val[j])*(w->gd->y->val[j]) 
        >= rad2
      ) {
        disp += (w->val)[i][j]*(w->val)[i][j];
        avrg += (w->val)[i][j];
        n++;
      }

  return sqrt(disp/n - (avrg/n)*(avrg/n));

}


EXT double wigner_max
  (wigner* w)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double max = -1/PI;

  wigner_loop(i,j,Nx,Ny)
    if ( w-> val[i][j] > max) 
      max = w-> val[i][j];

  return max;

}


EXT double wigner_min
  (wigner* w)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double min = 1/PI;

  wigner_loop(i,j,Nx,Ny)
    if ( w-> val[i][j] < min) 
      min = w-> val[i][j];

  return min;

}


EXT double wigner_overlap
  (wigner* w1, wigner* w2)
{

  int i, j,
      Nx = w1-> gd-> x-> n_val,
      Ny = w1-> gd-> y-> n_val;

  double over = 0.0;

  if_radial(w1-> gd)
    wigner_loop(i,j,Nx,Ny)
      over += w1-> gd-> x-> val[i] * w1-> val[i][j] * w2-> val[i][j];

  else
    wigner_loop(i,j,Nx,Ny)
      over += w1-> val[i][j] * w2-> val[i][j];

  return 2*PI * over * grid_cell(w1-> gd);

}


EXT double wigner_dist_over
  (wigner* w1, wigner* w2)
{

  return  wigner_overlap(w1,w2) / sqrt( wigner_purity(w1) * wigner_purity(w2) );

}


EXT double wigner_dist_l2
  (wigner* w1, wigner* w2)
{

  int i, j,
      Nx = w1-> gd-> x-> n_val,
      Ny = w1-> gd-> y-> n_val;

  double over = 0.0;

  if_radial(w1-> gd)
    wigner_loop(i,j,Nx,Ny) {
      double a = w1-> val[i][j] - w2-> val[i][j];
      over += w1-> gd-> x-> val[i] * a * a; 
    }

  else
    wigner_loop(i,j,Nx,Ny) {
      double a = w1-> val[i][j] - w2-> val[i][j];
      over += a * a; 
    }

  return sqrt( over * grid_cell(w1-> gd) );

}


EXT wigner*  wigner_linear
  (wigner** w_ary, double* c_ary, int size)
{

  wigner* w = wigner_new( w_ary[0]-> gd );
  wigner_to_zero(w);

  int i, j, k,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double over = 0;

  wigner_loop(i,j,Nx,Ny)
    loop(k,size)
      w-> val[i][j] += c_ary[k] * w_ary[k]-> val[i][j];

  return w;

}


///        keep it ???           ///
EXT double* wigner_x_marg
  (wigner* w)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double* m;

  if_cartesian(w->gd) {
    m = double_alloc(Nx);
    loop(i,Nx) {
      m[i] = 0;
      loop(j,Ny)
        m[i] += (w->val)[i][j];
    }
  }

  return m;

}


EXT double* wigner_p_marg
  (wigner* w)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double* m;

  if_cartesian(w->gd) {
    m = double_alloc(Ny);
    loop(j,Ny)
      m[j] = 0;
    loop(i,Nx)
      loop(j,Ny)
        m[j] += (w->val)[i][j];
  }

  return m;

}


EXT wigner* wigner_convolate
  (wigner* w, wigner* kernel)
{

  int i, j, a, b,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val,
      Kx = kernel-> gd-> x-> n_val,
      Ky = kernel-> gd-> y-> n_val;

  wigner* ww = wigner_new(w-> gd);
  wigner_to_zero(ww);

  wigner_loop(i,j,Nx,Ny) {
    wigner_loop(a,b,Kx,Ky) 
      if ( (kernel->val)[a][b] != 0.0) {

        int x_a = i - a + ( Kx - 1)/2;
        int y_b = j - b + ( Ky - 1)/2;

        if ( x_a >= 0 && y_b >= 0 && x_a < Nx && y_b < Ny)
          ww-> val[i][j] += w-> val[x_a][y_b] * kernel-> val[a][b];

      }
    ww-> val[i][j] *= grid_cell(kernel-> gd);
  }

  return ww;

}


EXT wigner* wigner_gaussian_conv
  (wigner* w, double var)
{

  double kernel_builder(double x, double y) 
    {return exp(-((x)*(x)+(y)*(y))/(2*var))/(2*PI*var);}

  wigner* kernel = wigner_generate(w->gd, kernel_builder);
  wigner_renorm(kernel);

  w->val = (wigner_convolate(w,kernel))->val;

}


/*
EXT void wigner_teleport
  (wigner* w, double r)
{
  double var = exp(-2*r);
  wigner_gaussian_conv(w,var);
}
*/


EXT void wigner_loss
  (wigner* w, double eta)
{

  int i, j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double var = (1.0-eta)/(2*eta);
  wigner_gaussian_conv(w,var);

  var = 1.0/eta;
  wigner_loop(i,j,Nx,Ny)
    w-> val[i][j] *= var;

  //double rescale(double x) 
    //{return x * sqrt(eta);}
  //grid_dilate(w-> gd,rescale);
  //wigner_renorm(w); 

}
///        \keep it             ///


EXT 
wigner* 
wigner_teleport
  (wigner* w1, double sigma, grid* g)
{

  wigner* w2 = wigner_new(g);

  int i,j,k,l,
      Ni = w1-> gd-> x-> n_val,
      Nj = w1-> gd-> y-> n_val,
      Nk = g-> x-> n_val,
      Nl = g-> y-> n_val;

  double  gam = 0.5 / (sigma*sigma),
          rez = grid_cell(w1-> gd),
         *xi  = w1-> gd-> x-> val,
         *yj  = w1-> gd-> y-> val,
         *xk  = g-> x-> val,
         *yl  = g-> y-> val,
        **v1  = w1-> val,
        **v2  = w2-> val;
        
  for(k = 0; k < Nk; k++)
  {
    double xxk = xk[k];
    for(l = 0; l < Nl; l++)
    {  
      double val = 0.0,
             yyl = yl[l];

      if_cartesian(w1-> gd) {
        wigner_loop(i,j,Ni,Nj)
          val += v1[i][j] * exp( - gam*( (xi[i] - xxk) * (xi[i] - xxk) + (yj[j] - yyl) * (yj[j] - yyl) ) );

      } else {
        for(j = 0; j < Nj; j++)
        {
          double  c = cos( yj[j] ),
                  s = sin( yj[j] );
          for(i = 0; i < Ni; i++)
          {
            double xx = (xi[i] * c - xxk) * (xi[i] * c - xxk),
                   yy = (xi[i] * s - yyl) * (xi[i] * s - yyl);
            val += xi[i] * v1[i][j] * exp( - gam*( xx + yy ) );
          }
        }

      }

      v2[k][l] = val * rez / (2*PI * sigma * sigma);

    }

  }

  return w2;

}


EXT 
double
wigner_teleport_neg
  (wigner* w, double sigma)
{

  int i,j,
      Nx = w-> gd-> x-> n_val,
      Ny = w-> gd-> y-> n_val;

  double  gamma   = 0.5 / (sigma*sigma),
          neg     = 0.0,
         *x       = w-> gd-> x-> val,
         *y       = w-> gd-> y-> val,
        **v       = w-> val,
         *gkernel = (double*) malloc( sizeof(double) * Nx );

  for (i = 0; i < Nx; i++)
    gkernel[i] = exp( - gamma*( x[i]*x[i] ) );

  if (w-> gd-> type == cartesian)
  {
    for(i = 0; i < Nx; i++)
    {
      double line = 0.0;
      for(j = 0; j < Ny; j++)
        line += v[i][j] * gkernel[j];
      neg += line * gkernel[i];
    }
  } else {
    for(i = 0; i < Nx; i++)
    {
      double rad = 0.0;
      for(j = 0; j < Ny; j++)
        rad += v[i][j];
      neg += rad * x[i] * gkernel[i];
    }
  }

  free(gkernel);

  return neg * grid_cell(w-> gd) * gamma / PI;

}


#undef wigner_loop


