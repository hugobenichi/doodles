#include "quix/generator/generator.h"

   
/*
            VACUUM
*/


static wigner* generator_vacuum_wigner
  (generator* gen, grid* g)
{

  double sample_xy(double x, double y)  
    { return exp(- x*x - y*y ) /PI; }

  double sample_rad(double x, double y) 
    { return exp(- x*x ) /PI;   }

  if ( g-> type == cartesian ) 
    return wigner_generate(g, sample_xy);
  else
    return wigner_generate(g, sample_rad);

}


static dmatrix* generator_vacuum_dmatrix
  (generator* gen, int s)
{

  dmatrix* d = dmatrix_new(s);

  dmatrix_to_zero(d);

  d-> re[0][0] = 1.0;

  return d;

}


static histogram* generator_vacuum_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double sample_xy(double x, double y)  { return exp(- x*x - y*y ) /PI; }

  double sample_hist(double phi, double q) {

    double p = 0.0, c = cos(phi), s = sin(phi);

    int i, stp = rg-> n_val;

    for (i = 0; i < stp; i++)
      p += sample_xy( q * c - rg-> val[i] * s, q * s + rg-> val[i] * c);

    return p*(rg-> resolution);

  }

  return (histogram*) wigner_generate(g, sample_hist);

}


static marginal* generator_vacuum_marginal
  (generator* gen, int n, double x_max )
{

  double vacuum_wavefunction(double x, double phi)
    { return exp( - x*x ) / sqrt(PI); }

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, vacuum_wavefunction, n, x_max, 1.0/sqrt(PI));

  return m;

}


static void generator_vacuum_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double vacuum_wavefunction(double x, double phi)
    { return exp( - x*x ) / sqrt(PI); }

  marginal_mc_proba_to_quad(quad, vacuum_wavefunction, n, x_max, 1.0/sqrt(PI));

}


EXT generator*  generator_vacuum
  ()
{

  generator* g = generator_new(1);

  g-> wig  = generator_vacuum_wigner;
  g-> dmat = generator_vacuum_dmatrix;
  g-> hist = generator_vacuum_histogram;
  g-> marg = generator_vacuum_marginal;
  g-> quad = generator_vacuum_quadrature;

  return g;

}


