#include "quix/generator/generator.h"


/*
            THERMAL
*/


static wigner* generator_thermal_wigner
  (generator* gen, grid* g)
{

  double a = 1.0 / ( 1.0 + 2.0 * gen-> parameters[0] );
  double b = a / PI;

  double sample_xy(double x, double y)  { return b * exp(- a * (x*x + y*y) ); }
  double sample_rad(double x, double y) { return b * exp(- a * x*x );   }

  if ( g-> type == cartesian ) 
    return wigner_generate(g, sample_xy);
  else
    return wigner_generate(g, sample_rad);

}


static dmatrix* generator_thermal_dmatrix
  (generator* gen, int s)
{

  double m = gen-> parameters[0];
  double a = 1.0 / ( 1.0 + m );
  double b = m / ( 1.0 + m );
  double c = 1.0;

  dmatrix* d = dmatrix_new(s);
  dmatrix_to_zero(d);

  int i;
  for (i = 0; i < s + 1; i++) {
    d-> re[i][i] = a * c;
    c *= b;
  }

  return d;

}


static histogram* generator_thermal_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double a = 1.0 / ( 1.0 + 2.0 * gen-> parameters[0] );
  double b = a / PI;

  double sample_xy(double x, double y)  { return b * exp(- a * (x*x + y*y) ); }

  double sample_hist(double phi, double q) {

    double p = 0.0, c = cos(phi), s = sin(phi);

    int i, stp = rg-> n_val;

    for (i = 0; i < stp; i++)
      p += sample_xy( q * c - rg-> val[i] * s, q * s + rg-> val[i] * c);

    return p*(rg-> resolution);

  }

  return (histogram*) wigner_generate(g, sample_hist);

}


static marginal* generator_thermal_marginal
  (generator* gen, int n, double x_max )
{

  double a = 1.0 / ( 1.0 + 2.0 * gen-> parameters[0] );
  double b = sqrt(a / PI);

  double thm_wavefunction(double x, double phi)
    { return b * exp( - a * x*x); }

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, thm_wavefunction, n, x_max, b);

  return m;

}


static void generator_thermal_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double a = 1.0 / ( 1.0 + 2.0 * gen-> parameters[0] );
  double b = sqrt(a / PI);

  double thm_wavefunction(double x, double phi)
    { return b * exp( - a * x*x); }

  marginal_mc_proba_to_quad(quad, thm_wavefunction, n, x_max, b);

}


EXT generator*  generator_thermal
  (double n_mean)
{

  generator* g = generator_new(3);

  g-> parameters[0] = n_mean;
  g-> parameters[1] = 1.0 / ( 1.0 + 2.0 * n_mean );
  g-> parameters[2] = sqrt( g-> parameters[1] / PI);

  g-> wig  = generator_thermal_wigner;
  g-> dmat = generator_thermal_dmatrix;
  g-> hist = generator_thermal_histogram;
  g-> marg = generator_thermal_marginal;
  g-> quad = generator_thermal_quadrature;

  return g;

}

EXT generator*  generator_thm_tele
  (double n_mean, double r)
{

  return generator_thermal( n_mean + exp( -2*r) ); 

}

EXT generator*  generator_vac_tele
  (double r)
{

  return generator_thermal( exp( -2*r) );

}


