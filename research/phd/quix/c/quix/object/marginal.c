#include "quix/object/marginal.h"


EXT marginal* marginal_new
  (int sample_size)
{

  marginal* m = quix_object_alloc(marginal);

  m-> samples = sample_size;
  m-> quad    = double_alloc(sample_size);
  m-> phase   = double_alloc(sample_size);
  alloc_check(m-> quad ,"marginal_new");
  alloc_check(m-> phase,"marginal_new");

  return m;

}


EXT marginal* marginal_copy
  (int sample_size, double* quad, double* phase)
{

  marginal* m = marginal_new(sample_size);

  int i;
  loop(i, sample_size) {
    m-> quad[i]  = quad[i];  
    m-> phase[i] = phase[i];  
  }

  return m;

}


EXT void  marginal_free
  (marginal* m)
{
  
  if ( quix_object_free(m) )
  {
    if (m-> quad && m-> phase) 
    {
      free( m-> quad );
      free( m-> phase);
      m-> quad  = NULL;
      m-> phase = NULL;
    } else already_null("Marginal");
    free(m);
  } else already_free("Marginal");
  
}


EXT histogram* marginal_to_raw_histogram
  (marginal* m, grid* g)
{

  if_radial(g)    grid_hist(g);
  if_cartesian(g) grid_hist(g);

  histogram* h = histogram_new(g);
  wigner_to_zero((wigner*)h);

  int i, 
      M   = m-> samples, 
      n_q = (h-> gd ->y -> n_val + 1) / 2,
      Nx  = h-> gd-> x-> n_val,
      Ny  = h-> gd-> y-> n_val;

  double r_x = h-> gd-> x-> resolution,
         r_y = h-> gd-> y-> resolution;

  loop(i,M){

    int p_i =       (int)round( m-> quad[i]  / r_x ),
        q_i = n_q + (int)round( m-> phase[i] / r_y );

    if (p_i == g-> x-> n_val) p_i = 0;  //necessary to guard against phase = pi exactly

    h-> val[p_i][q_i] += 1.0;

  }

  loop(i,Ny)
    h-> val[Nx - 1][Ny - i - 1] = h-> val[0][i];

  return h;

}


double marginal_draw_gaussian
  (double sigma)
{

  double r     = (double)(RAND_MAX) + (double)(1),
         x_max = 8.0 * sigma,
         x     = sigma,
         y     = 1.0,
         a     = 0.5/(sigma*sigma),
         b     = (1.0/sqrt(2*PI))/sigma;
  //double r_x   = 2.0 * x_max/r;
  double r_x   = 2.0 * x_max/r; 

  reinit_seed();
 
  while (y > b*exp(-a*x*x)) {
    x = r_x * rand() - x_max;
    y = 1.0 * rand() / r;
  }

  return x;

}


EXT void marginal_phase_noise
  (marginal *m, double sigma)
{

  int i, M = m-> samples;
  loop(i,M)
    m->phase[i] += marginal_draw_gaussian(sigma);

}


EXT void marginal_quad_noise
  (marginal *m, double sigma)
{

  int i, M = m-> samples;
  loop(i,M)
    m->quad[i] += marginal_draw_gaussian(sigma);

}


EXT marginal* marginal_teleportation
  (marginal *m, double r)
{

  int i, M = m-> samples;
  double sigma = exp(-r);
  
  marginal* new_m = marginal_new(M);

  loop(i,M) {
    new_m->phase[i] = m->phase[i];
    new_m->quad[i]  = m->quad[i] + marginal_draw_gaussian(sigma);
  }

  return new_m;

}


EXT marginal* marginal_loss
  (marginal *m, double loss)
{

  int i, M = m-> samples;
  double  sigma = 1.0/sqrt(2.0),
          a     = sqrt(1-loss),
          b     = sqrt(loss);
  
  marginal* new_m = marginal_new(M);

  loop(i,M) {
    new_m->phase[i] = m->phase[i];
    new_m->quad[i] = a * m->quad[i] + b * marginal_draw_gaussian(sigma);
  }

  return new_m;

}


EXT marginal* marginal_mc_bootstrap
  (marginal* m, int n)
{

  marginal* new_m = marginal_new(n);

  int i, j, M = m-> samples;

  reinit_seed();

  loop(i,n) {
    j = ( (int) rand() ) % M;
    new_m-> quad[i]  = m-> quad[j];
    new_m-> phase[i] = m-> phase[j];
  }

  return new_m;

}


EXT void marginal_mc_proba
  (marginal* m, probability w, int n, double x_max, double y_max)
{

  double r     = ( (double) RAND_MAX ) + 1.0;
  double r_x   = x_max / r * 2,
         r_y   = y_max / r,
         r_phi = 2 * PI / r;

  reinit_seed();

  int i = 0;
  while (i < n){

    double x   = rand() * r_x  - x_max;
    double y   = rand() * r_y;
    double phi = rand() * r_phi;

    if ( y <= w(x,phi) ){
      m-> quad[i]  = x;
      m-> phase[i] = phi;
      i++;
    }

  }

}


EXT void marginal_mc_proba_to_quad
  (double* quad, probability w, int n, double x_max, double y_max)
{

  double r     = ( (double) RAND_MAX ) + 1.0;
  double r_x   = x_max / r * 2,
         r_y   = y_max / r,
         r_phi = 2 * PI / r;

  reinit_seed();

  int i = 0;
  while (i < n){

    double x   = rand() * r_x  - x_max;
    double y   = rand() * r_y;
    double phi = rand() * r_phi;

    if ( y <= w(x,phi) )
      quad[i++]  = x;

  }

}


EXT marginal* marginal_mc_histogram 
  (histogram* h, int n_samples, double max)
{

  #define H(h,i,j)        ( (h) ->val[i][j] )
  #define Inter(h,i,j)    ( H(h,i,j) + H(h,i+1,j) + H(h,i,j+1) + H(h,i+1,j+1) ) / 4

  marginal* m = marginal_new(n_samples);

  double r = (double)(RAND_MAX)+(double)(1);

  double phi_max = h-> gd-> x-> val[h-> gd-> x-> n_val - 1], r_phi = phi_max / r;
  double x_max   = h-> gd-> y-> val[h-> gd-> y-> n_val - 1], r_x   = 2 * x_max / r;
  double y_max   = wigner_max( (wigner*) h );
  double r_y     = y_max / r;
  double phi, x, y;
  int i = 0, q_i, p_i, n_q = (h-> gd-> y-> n_val + 1) / 2 ;

  while (i < n_samples){

    phi = rand() * r_phi;
    x   = rand() * r_x  - x_max;
    y   = rand() * r_y;

    p_i =       (int) floor( phi / (h-> gd-> x-> resolution) );
    q_i = n_q + (int) floor( x   / (h-> gd-> y-> resolution) );

    if ( y <= Inter(h, p_i, q_i) ){

      m-> quad[i]  = x;
      m-> phase[i] = phi;
      i++;

    }

  }

  #undef H
  #undef Inter

  return m;


}


