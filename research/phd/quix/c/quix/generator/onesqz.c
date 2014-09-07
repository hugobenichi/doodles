#include "quix/generator/generator.h"


/*
            SQUEEZED PHOTON
*/


static wigner* generator_sqzd_photon_wigner
  (generator* gen, grid* g)
{

  double e  = exp( gen-> parameters[0] );
  double ee = e*e;
  double ff = 1.0 / ee;
  double rr;

  double sample_xy(double x, double y)  
  { 
    rr = ee * x*x + ff * y*y ;
    return 2 * (rr - 0.5) * exp(- rr ) /PI; 
  }

  double sample_rad(double x, double y) 
  { 
    double c  = cos(y);
    double s  = sin(y);
    double cc = c*c;
    double ss = s*s;
    rr      = x*x * ( ee * cc + ff * ss ) ;
    return 2 * (rr - 0.5) * exp(- rr ) /PI;   
  }

  if ( g-> type == cartesian ) return wigner_generate(g, sample_xy);

  return wigner_generate(g, sample_rad);

}


static dmatrix* generator_sqzd_photon_dmatrix
  (generator* gen, int n)
{

  double s = gen-> parameters[0];

  dmatrix* m = dmatrix_new(n);
  dmatrix_to_zero(m);

  m->re[1][1]  = 1.0/cosh(-s);
  m->re[1][1] *= 1.0/sinh(-s);
  m->re[1][1] *= 1.0/sinh(-s);
  double a =  tanh( -s ) ;

  int i,j;

  for (j = 1; 2*j < s; j++) 
      m->re[1][2*j+1] = (sqrt(2*j+1)/sqrt(2*j))*a*(m->re[1][2*j-1]);

  for (i = 1; 2*i < s; i++) {
    m->re[2*i+1][1] = (sqrt(2*i+1)/sqrt(2*i))*a*(m->re[2*i-1][1]); 

    for (j = 1; 2*j < s; j++)
      m->re[2*i+1][2*j+1] = (sqrt(2*j+1)/sqrt(2*j))*a*(m->re[2*i+1][2*j-1]);
  } 

  dmatrix_renorm(m);

  return m;

}


static histogram* generator_sqzd_photon_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double e  = exp( gen-> parameters[0] );
  double ee = e*e;
  double ff = 1.0/ee;
  double rr;

  double sample_xy(double x, double y)  
  { 
    rr = ee * x*x + ff * y*y ;
    return 2 * (rr - 0.5) * exp(- rr ) /PI; 
  }

  double sample_hist(double phi, double q) {

    double p = 0.0, c = cos(phi), s = sin(phi);

    int i, stp = rg-> n_val;

    for (i = 0; i < stp; i++)
      p += sample_xy( q * c - rg-> val[i] * s, q * s + rg-> val[i] * c);

    return p * rg-> resolution;

  }

  return (histogram*) wigner_generate(g, sample_hist);

}


static marginal* generator_sqzd_photon_marginal
  (generator* gen, int n, double x_max )
{


  double s = 2 * gen-> parameters[0];
  double a = exp(s);
  double b = 1.0 / a;
  double t = sinh(s);

  double sqzd_one_wavefunction(double x, double phi)
  { 
    double c = cos(phi);
    double s = sin(phi);
    double cc = c*c;
    double ss = s*s;
    double g = a * cc + b * ss; 
    double h = b * cc + a * ss; 
    double j = sin(s)*t;
    double xx = x*x;
    double n = (g*h - j*j) / h;
    return 2 * xx * n * exp( - n * xx ) / sqrt(h) / sqrt(PI); 
  }

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, sqzd_one_wavefunction, n, x_max, 1.0);

  return m;

}


static void generator_sqzd_photon_quadrature
  (generator* gen, double* quad, int n, double x_max )
{


  double s = 2 * gen-> parameters[0];
  double a = exp(s);
  double b = 1.0 / a;
  double t = sinh(s);

  double sqzd_one_wavefunction(double x, double phi)
  { 
    double c = cos(phi);
    double s = sin(phi);
    double cc = c*c;
    double ss = s*s;
    double g = a * cc + b * ss; 
    double h = b * cc + a * ss; 
    double j = sin(s)*t;
    double xx = x*x;
    double n = (g*h - j*j) / h;
    return 2 * xx * n * exp( - n * xx ) / sqrt(h) / sqrt(PI); 
  }

  marginal_mc_proba_to_quad(quad, sqzd_one_wavefunction, n, x_max, 1.0);

}


EXT generator* generator_sqzd_photon
  (double s)
{

  generator* g = generator_new(1);
  g-> parameters[0] = s;

  g-> wig  = generator_sqzd_photon_wigner;
  g-> dmat = generator_sqzd_photon_dmatrix;
  g-> hist = generator_sqzd_photon_histogram;
  g-> marg = generator_sqzd_photon_marginal;
  g-> quad = generator_sqzd_photon_quadrature;

  return g;

}


