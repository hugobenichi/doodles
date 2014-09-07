#include "quix/generator/generator.h"

   
/*
            COHERENT
*/


static wigner* generator_coherent_wigner
  (generator* gen, grid* g)
{

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 

  double sample_xy(double x, double y)  
  { 
    return exp(- (x - xo) * (x - xo) - (y - yo) * (y - yo) ) /PI; 
  }

  double sample_rad(double x, double y) 
  { 
    double xc = x * cos(y) - xo;
    double xs = x * sin(y) - yo;
    return exp(- xc*xc - xs*xs ) /PI;   
  }

  if ( g-> type == cartesian ) 
    return wigner_generate(g, sample_xy);
  else
    return wigner_generate(g, sample_rad);

}


static dmatrix* generator_coherent_dmatrix
  (generator* gen, int s)
{

  int i,j;
  double a, b;

  double x  = gen-> parameters[0];
  double y  = gen-> parameters[1];  

  double xx = x*x;
  double yy = y*y;

  dmatrix* m = dmatrix_new(s);
  dmatrix_to_zero(m);

  m->re[0][0] = exp(-( xx + yy )/2.0 );
  m->im[0][0] = 0.0;

  for (j = 1; j < s + 1; j++) {
      b = 1.0/sqrt(2.0*j);
      m->re[0][j] = b*( x * (m->re[0][j-1]) + y * (m->im[0][j-1]) );
      m->im[0][j] = b*( x * (m->im[0][j-1]) - y * (m->re[0][j-1]) );
  }

  for (i = 1; i < s + 1; i++) {

    a = 1.0/sqrt(2.0*i);
    m->re[i][0] = a*( x * (m->re[i-1][0]) - y * (m->im[i-1][0]) );
    m->im[i][0] = a*( x * (m->im[i-1][0]) + y * (m->re[i-1][0]) );

    for (j = 1; j < s + 1; j++) {
      b = 1.0/sqrt(2.0*j);
      m->re[i][j] = b*( x * (m->re[i][j-1]) + y * (m->im[i][j-1]) );
      m->im[i][j] = b*( x * (m->im[i][j-1]) - y * (m->re[i][j-1]) );
    }  

  } 

  return m;

}


static histogram* generator_coherent_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 

  double sample_xy(double x, double y)  
  { 
    return exp(- (x - xo) * (x - xo) - (y - yo)*(y - yo) ) /PI; 
  }

  double sample_hist(double phi, double q) {

    double p = 0.0, c = cos(phi), s = sin(phi);

    int i, stp = rg-> n_val;

    for (i = 0; i < stp; i++)
      p += sample_xy( q * c - rg-> val[i] * s, q * s + rg-> val[i] * c);

    return p*(rg-> resolution);

  }

  return (histogram*) wigner_generate(g, sample_hist);

}


static marginal* generator_coherent_marginal
  (generator* gen, int n, double x_max )
{

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 

  double coh_wavefunction(double x, double phi)
  { 
    double r = x - xo * cos(phi) - yo * sin(phi);
    return exp( - r*r ) / sqrt(PI); 
  }

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, coh_wavefunction, n, x_max, 1.0/sqrt(PI));

  return m;

}


static void generator_coherent_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 

  double coh_wavefunction(double x, double phi)
  { 
    double r = x - xo * cos(phi) - yo * sin(phi);
    return exp( - r*r ) / sqrt(PI); 
  }

  marginal_mc_proba_to_quad(quad, coh_wavefunction, n, x_max, 1.0/sqrt(PI));

}


EXT generator*  generator_coherent
  (double x, double y)
{

  generator* g = generator_new(2);
  g-> parameters[0] = x;
  g-> parameters[1] = y;
 
  g-> wig  = generator_coherent_wigner;
  g-> dmat = generator_coherent_dmatrix;
  g-> hist = generator_coherent_histogram;
  g-> marg = generator_coherent_marginal;
  g-> quad = generator_coherent_quadrature;

  return g;

}


/*
          COHERENT TELE
*/


static wigner* generator_coh_tele_wigner
  (generator* gen, grid* g)
{

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1];
  double nm  = 1.0 / (1.0 +  2 * exp( -2 * gen-> parameters[2]) );  

  double sample_xy(double x, double y)  
  { 
    double xx = x - xo;
    double yy = y - yo;

    return nm * exp(- nm * ( xx*xx + yy*yy ) ) /PI; 
  }

  double sample_rad(double x, double y) 
  { 
    double xc = x * cos(y) - xo;
    double xs = x * sin(y) - yo;
    return nm * exp(- nm * ( xc*xc + xs*xs ) ) /PI;   
  }

  if ( g-> type == cartesian ) 
    return wigner_generate(g, sample_xy);
  else
    return wigner_generate(g, sample_rad);

}


static dmatrix* generator_coh_tele_dmatrix
  (generator* gen, int s)
{

  int i,j;
  double a, b;

  double x  = gen-> parameters[0];
  double y  = gen-> parameters[1];  
  double tl  = 2 * exp( -2 * gen-> parameters[2]);  

  double xx = x*x;
  double yy = y*y;

  dmatrix* m = dmatrix_new(s);
  dmatrix_to_zero(m);

  m->re[0][0] = exp(-( xx + yy )/2.0 );
  m->im[0][0] = 0.0;

  for (j = 1; j < s + 1; j++) {
      b = 1.0/sqrt(2.0*j);
      m->re[0][j] = b*( x * (m->re[0][j-1]) + y * (m->im[0][j-1]) );
      m->im[0][j] = b*( x * (m->im[0][j-1]) - y * (m->re[0][j-1]) );
  }

  for (i = 1; i < s + 1; i++) {

    a = 1.0/sqrt(2.0*i);
    m->re[i][0] = a*( x * (m->re[i-1][0]) - y * (m->im[i-1][0]) );
    m->im[i][0] = a*( x * (m->im[i-1][0]) + y * (m->re[i-1][0]) );

    for (j = 1; j < s + 1; j++) {
      b = 1.0/sqrt(2.0*j);
      m->re[i][j] = b*( x * (m->re[i][j-1]) + y * (m->im[i][j-1]) );
      m->im[i][j] = b*( x * (m->im[i][j-1]) - y * (m->re[i][j-1]) );
    }  

  } 

  return m;

}


static histogram* generator_coh_tele_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 
  double nm  = 1.0 / (1.0 +  2 * exp( -2 * gen-> parameters[2]) );  

  double sample_xy(double x, double y)  
  { 
    double xx = x - xo;
    double yy = y - yo;

    return nm * exp(- nm * ( xx*xx + yy*yy ) ) /PI; 
  }

  double sample_hist(double phi, double q) {

    double p = 0.0, c = cos(phi), s = sin(phi);

    int i, stp = rg-> n_val;

    for (i = 0; i < stp; i++)
      p += sample_xy( q * c - rg-> val[i] * s, q * s + rg-> val[i] * c);

    return p*(rg-> resolution);

  }

  return (histogram*) wigner_generate(g, sample_hist);

}


static marginal* generator_coh_tele_marginal
  (generator* gen, int n, double x_max )
{

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1];
  double nm  = 1.0 / (1.0 +  2 * exp( -2 * gen-> parameters[2]) );

  double coh_wavefunction(double x, double phi)
  { 
    double r = x - xo * cos(phi) - yo * sin(phi);
    return nm * exp( - nm * r*r ) / sqrt(PI); 
  }

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, coh_wavefunction, n, x_max, 1.0/sqrt(PI));

  return m;

}


static void generator_coh_tele_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 
  double nm  = 1.0 / (1.0 +  2 * exp( -2 * gen-> parameters[2]) );

  double coh_wavefunction(double x, double phi)
  { 
    double r = x - xo * cos(phi) - yo * sin(phi);
    return nm * exp( - nm * r*r ) / sqrt(PI); 
  }

  marginal_mc_proba_to_quad(quad, coh_wavefunction, n, x_max, 1.0/sqrt(PI));

}


EXT generator*  generator_coh_tele
  (double x, double y, double r)
{

  generator* g = generator_new(3);
  g-> parameters[0] = x;
  g-> parameters[1] = y;
  g-> parameters[2] = r;
  
  g-> wig  = generator_coh_tele_wigner;
  g-> dmat = generator_coh_tele_dmatrix;
  g-> hist = generator_coh_tele_histogram;
  g-> marg = generator_coh_tele_marginal;
  g-> quad = generator_coh_tele_quadrature;

  return g;

}


EXT generator*  generator_coh_loss_tele
  (double x, double y, double eta, double r)
{

  return generator_coh_tele(sqrt(eta) * x, sqrt(eta) * y, r);

}


