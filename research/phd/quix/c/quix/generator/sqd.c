#include "quix/generator/generator.h"


/*
            SQUEEZING
*/


static wigner* generator_squeezing_wigner
  (generator* gen, grid* g)
{

  double a = gen-> parameters[0];
  double b = gen-> parameters[1];

  double sample_xy(double x, double y)  
  { 
    return exp(- a * x*x - b * y*y ) /PI; 
  }

  double sample_rad(double x, double y) 
  { 
    double xc = x*cos(y);
    double xs = x*sin(y);
    return exp(- a * xc*xc - b * xs*xs ) /PI;   
  }

  if ( g-> type == cartesian ) 
    return wigner_generate(g, sample_xy);
  else
    return wigner_generate(g, sample_rad);

}


static dmatrix* generator_squeezing_dmatrix
  (generator* gen, int n)
{

  dmatrix* m = dmatrix_new(n);
  dmatrix_to_zero(m);

  double s = 0.5 * log( gen-> parameters[0]);

  m->re[0][0] = 1.0/cosh( - s );
  double a =  tanh( - s )/2 ;

  int i,j;

  for (j = 1; 2*j < n + 1; j++) 
    m-> re[0][2*j] = sqrt(2*j*(2*j-1)) * a * (m-> re[0][2*j-2])/j;

  for (i = 1; 2*i < n + 1; i++) {

    m-> re[2*i][0] = sqrt(2*i*(2*i-1)) * a * (m-> re[2*i-2][0])/i; 

    for (j = 1; 2*j < n + 1; j++)
      m-> re[2*i][2*j] = sqrt(2*j*(2*j-1)) * a * (m-> re[2*i][2*j-2])/j;

  } 

  return m;

}


static histogram* generator_squeezing_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double a = gen-> parameters[0];
  double b = gen-> parameters[1];

  double sample_xy(double x, double y)  
  { 
    return exp(- a * x*x - b * y*y ) /PI; 
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


static marginal* generator_squeezing_marginal
  (generator* gen, int n, double x_max )
{

  double a = gen-> parameters[0];
  double b = gen-> parameters[1];

  double sqzd_wavefunction(double x, double phi)
  { 
    double g  = 2.0 / ( a + b + (b-a)*cos(2*phi) );

    return  sqrt(g) * exp( - g * x*x  ) / sqrt(PI); 

  }

  double max = a > b ? a : b;

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, sqzd_wavefunction, n, x_max, max);

  return m;

}


static void generator_squeezing_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double a = gen-> parameters[0];
  double b = gen-> parameters[1];

  double sqzd_wavefunction(double x, double phi)
  { 
    double g  = 2.0 / ( a + b + (b-a)*cos(2*phi) );
    return  sqrt(g) * exp( - g * x*x  ) / sqrt(PI); 

  }

  double max = a > b ? a : b;

  marginal_mc_proba_to_quad(quad, sqzd_wavefunction, n, x_max, max);

}


EXT generator*  generator_squeezing
  (double s)
{

  generator* g = generator_new(2);
  g-> parameters[0] = exp(-2*s);
  g-> parameters[1] = exp(+2*s);
 
  g-> wig  = generator_squeezing_wigner;
  g-> dmat = generator_squeezing_dmatrix;
  g-> hist = generator_squeezing_histogram;
  g-> marg = generator_squeezing_marginal;
  g-> quad = generator_squeezing_quadrature;

  return g;

}


/*
            SQUEEZING TELE
*/


static wigner* generator_sqz_tele_wigner
  (generator* gen, grid* g)
{

  double a = gen-> parameters[0];
  double b = gen-> parameters[1];
  double n = sqrt( a*b );

  double sample_xy(double x, double y)  
  { 
    return n * exp(- a * x*x - b * y*y ) /PI; 
  }

  double sample_rad(double x, double y) 
  { 
    double xc = x*cos(y);
    double xs = x*sin(y);
    return n * exp(- a * xc*xc - b * xs*xs ) /PI;   
  }

  if ( g-> type == cartesian ) 
    return wigner_generate(g, sample_xy);
  else
    return wigner_generate(g, sample_rad);

}


static dmatrix* generator_sqz_tele_dmatrix
  (generator* gen, int s)
{

  dmatrix* m = dmatrix_new(s);
  dmatrix_to_zero(m);

  m->re[0][0] = 1.0/cosh( - gen-> parameters[0] );
  double a =  tanh( - gen-> parameters[0] )/2 ;

  int i,j;

  for (j = 1; 2*j < s + 1; j++) 
    m-> re[0][2*j] = sqrt(2*j*(2*j-1)) * a * (m-> re[0][2*j-2])/j;

  for (i = 1; 2*i < s + 1; i++) {

    m-> re[2*i][0] = sqrt(2*i*(2*i-1)) * a * (m-> re[2*i-2][0])/i; 

    for (j = 1; 2*j < s + 1; j++)
      m-> re[2*i][2*j] = sqrt(2*j*(2*j-1)) * a * (m-> re[2*i][2*j-2])/j;

  } 

  return m;

}


static histogram* generator_sqz_tele_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double a = gen-> parameters[0];
  double b = gen-> parameters[1];
  double n = sqrt( a*b );

  double sample_xy(double x, double y)  
  { 
    return n * exp(- a * x*x - b * y*y ) /PI; 
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


static marginal* generator_sqz_tele_marginal
  (generator* gen, int n, double x_max )
{

  double a = gen-> parameters[0];
  double b = gen-> parameters[1];
  double t = 2.0 * a * b;

  double sqzd_wavefunction(double x, double phi)
  { 
    double g  = t / ( a + b + (b-a)*cos(2*phi) );

    return  sqrt(g) * exp( - g * x*x  ) / sqrt(PI); 

  }

  double max = a > b ? a : b;

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, sqzd_wavefunction, n, x_max, max);

  return m;

}


static void generator_sqz_tele_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double a = gen-> parameters[0];
  double b = gen-> parameters[1];
  double m = 2.0 * a  *b;

  double sqzd_wavefunction(double x, double phi)
  { 
    double g  = m / ( a + b + (b-a)*cos(2*phi) );

    return  sqrt(g) * exp( - g * x*x  ) / sqrt(PI); 

  }

  double max = a > b ? a : b;

  marginal_mc_proba_to_quad(quad, sqzd_wavefunction, n, x_max, max);

}


EXT generator*  generator_sqz_tele
  (double s, double r)
{

  generator* g = generator_new(2);
  g-> parameters[0] = 1.0 / ( exp(+2*s) + 2* exp(-2*r) );
  g-> parameters[1] = 1.0 / ( exp(-2*s) + 2* exp(-2*r) );
 
  g-> wig  = generator_sqz_tele_wigner;
  g-> dmat = generator_sqz_tele_dmatrix;
  g-> hist = generator_sqz_tele_histogram;
  g-> marg = generator_sqz_tele_marginal;
  g-> quad = generator_sqz_tele_quadrature;

  return g;

}


EXT generator*  generator_sqz_loss_tele
  (double s, double eta, double r)
{

  generator* g = generator_sqz_tele(s, r);

  g-> parameters[0] = 1.0 / ( 1.0 - eta + eta * exp(+2*s) + 2* exp(-2*r) );
  g-> parameters[1] = 1.0 / ( 1.0 - eta + eta * exp(-2*s) + 2* exp(-2*r) );
  
  return g;

}


