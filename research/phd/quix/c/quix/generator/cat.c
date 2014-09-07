#include "quix/generator/generator.h"

   
/*
           CAT
*/


static wigner* generator_cat_wigner
  (generator* gen, grid* g)
{

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 
  double phi = gen-> parameters[2]; 
  double f = exp( -xo*xo - yo*yo);
  double norm = 1.0/(1 + cos(phi) * f);

  double sample_xy(double x, double y)  
  { 
    return norm * exp(-x*x-y*y) * ( f*cosh(2*x*xo + 2*y*yo) + cos(2*x*yo - 2*y*xo + phi) )/PI; 
  }

  double sample_rad(double x, double y) 
  { 
    double xc = x*cos(y);
    double xs = x*sin(y);
    return norm * exp(-x*x) * ( f*cosh(2*xc*xo + 2*xs*yo) + cos(2*xc*yo - 2*xs*xo + phi) )/PI; 
  }

  if ( g-> type == cartesian ) 
    return wigner_generate(g, sample_xy);
  else
    return wigner_generate(g, sample_rad);

}


static dmatrix* generator_cat_dmatrix
  (generator* gen, int s)
{

  int i, j;

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 
  double phi = gen-> parameters[2]; 

  double sign;
  double c_phi = cos(phi); 
  double s_phi = sin(phi);
  double a = xo * xo + yo * yo;
  double b = 0.5 * exp(-a/2.0 ) / ( 1.0 + c_phi * exp(-a) );

  dmatrix* m = dmatrix_new(s);
  dmatrix_to_zero(m);

  m->re[0][0] = 2 * b * ( 1.0 + c_phi );
  m->im[0][0] = 0.0;

  m->re[1][1] = a * b * c_phi;
  m->im[1][1] = 0.0;

  for (i = 2; i < s + 1; i++) {
    m->re[i][i] = 0.25 * m->re[i-2][i-2] * a * a / (i*(i-1));
    m->im[i][i] = 0.0;
  }
    
/* 
  for (j = 1; j < s + 1; j++) {

      b = 1.0/sqrt(2.0 * j);
      m-> re[0][j] = b * ( xo * m-> re[0][j-1] + yo * m-> im[0][j-1] );
      m-> im[0][j] = b * ( xo * m-> im[0][j-1] - yo * m-> re[0][j-1] );

      sign = (j%2)? -1.0 : 1.0;
      b            = ( (1 + sign * c_phi) * m-> re[0][j] + sign * s_phi * m-> im[0][j] );
      m-> im[0][j] = ( (1 + sign * c_phi) * m-> im[0][j] - sign * s_phi * m-> re[0][j] );
      m-> re[0][j] = b;

  }

  for (i = 1; i < s + 1; i++) {

    a = 1.0/sqrt(2.0*i);
    m-> re[i][0] = a * ( xo * m-> re[i-1][0] - yo * m-> im[i-1][0] );
    m-> im[i][0] = a * ( xo * m-> im[i-1][0] + yo * m-> re[i-1][0] );

    sign = (i%2)? -1.0 : 1.0;
    a            = ( (1 + sign * c_phi) * m-> re[i][0] - sign * s_phi * (m-> im[i][0]) );
    m-> im[i][0] = ( (1 + sign * c_phi) * m-> im[i][0] + sign * s_phi * (m-> re[i][0]) );
    m-> re[i][0] = a;

    for (j = 1; j < s + 1; j++) {

      b = 1.0/sqrt(2.0*j);
      m-> re[i][j] = b * ( xo * (m-> re[i][j-1]) + yo * m-> im[i][j-1] );
      m-> im[i][j] = b * ( xo * (m-> im[i][j-1]) - yo * m-> re[i][j-1] );

      sign = (j%2)? -1.0 : 1.0;
      b            = ( (1 + sign * c_phi) * m->re[i][j] + sign * s_phi * m-> im[i][j] );
      m-> im[i][j] = ( (1 + sign * c_phi) * m->im[i][j] - sign * s_phi * m-> re[i][j] );
      m-> re[i][j] = b;

    }  

  } 
*/

  //dmatrix_renorm(m);

  return m;

}


static histogram* generator_cat_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 
  double phi = gen-> parameters[2]; 
  double f = exp( -xo*xo - yo*yo);
  double norm = 1.0/(1 + cos(phi) * f);

  double sample_xy(double x, double y)  
  { 
    return norm * exp(-x*x-y*y) * ( f*cosh(2*x*xo + 2*y*yo) + cos(2*x*yo - 2*y*xo + phi) )/PI; 
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


static marginal* generator_cat_marginal
  (generator* gen, int n, double x_max )
{

  double xo  = gen-> parameters[0];
  double po  = gen-> parameters[1]; 
  double phi = gen-> parameters[2] - PI; 
  double xoo = xo * xo;
  double poo = po * po;
  double f = exp( - xoo - poo);
  double norm = 2.0/( (1 + cos(phi) * f) * sqrt(PI) );
/*
  double cat_wavefunction(double x, double theta)
  { 
    double c = cos(theta);
    double s = sin(theta);
    double xoo = xo * c - yo * s;
    double yoo = yo * c + xo * s;
    double a = cos( x*yoo - phi/2 ) * cosh( x*xoo );
    double b = sin( x*yoo - phi/2 ) * sinh( x*yoo );
    return norm * exp( - x*x - xoo*xoo ) * ( a*a + b*b ) ; 
  }
*/
  double cat_wavefunction(double x, double theta)
  { 
    double c  = cos(theta);
    double s  = sin(theta);
    double s2 = sin(2*theta);

    double a = exp(+c*c*poo + s*s*xoo - s2*xo*po) * cosh( 2*x*(xo*c + po*s) );
    double b = exp(-c*c*xoo - s*s*poo + s2*xo*po) * cos(  2*x*(xo*s - po*c) + phi );


    return norm * exp( - x*x ) * ( f * a - b ) ; 
  }

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, cat_wavefunction, n, x_max, 1.0);

  return m;

}


static void generator_cat_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double xo  = gen-> parameters[0];
  double yo  = gen-> parameters[1]; 
  double phi = gen-> parameters[2]; 
  double xoo = xo * xo;
  double yoo = yo * yo;
  double f = exp( - xo*xo - yo*yo);
  double norm = 2.0/(1 + cos(phi) * f)/ sqrt(PI);
/*
  double cat_wavefunction(double x, double theta)
  { 
    double c = cos(theta);
    double s = sin(theta);
    double xoo = xo * c - yo * s;
    double yoo = yo * c + xo * s;
    double a = cos( x*yoo - phi/2 ) * cosh( x*xoo );
    double b = sin( x*yoo - phi/2 ) * sinh( x*yoo );
    return norm * exp( - x*x - xoo*xoo ) * ( a*a + b*b ) ; 
  }
*/
  double cat_wavefunction(double x, double theta)
  { 
    double c  = cos(theta);
    double s  = sin(theta);
    double s2 = sin(2*theta);

    double a = exp(+c*c*yoo + s*s*xoo - s2 * xo*yo) * cosh( 2*x*(xo * c + yo * s) );
    double b = exp(-c*c*xoo - s*s*yoo + s2 * xo*yo) * cos(  2*x*(yo * c - xo * s) + phi );


    return norm * f * exp( - x*x ) * ( a - b ) ; 
  }

  marginal_mc_proba_to_quad(quad, cat_wavefunction, n, x_max, 1.0);

}


EXT generator*  generator_cat
  (double x, double y, double phi)
{

  generator* g = generator_new(3);
  g-> parameters[0] = x;
  g-> parameters[1] = y;
  g-> parameters[2] = phi;
 
  g-> wig  = generator_cat_wigner;
  g-> dmat = generator_cat_dmatrix;
  g-> hist = generator_cat_histogram;
  g-> marg = generator_cat_marginal;
  g-> quad = generator_cat_quadrature;

  return g;

}


