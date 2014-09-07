#include "quix/generator/generator.h"

   
/*
            FOCK = 1
*/


static wigner* generator_one_wigner
  (generator* gen, grid* g)
{

  double rr;

  double sample_xy(double x, double y)  
  { 
    rr = x*x + y*y;
    return 2 * (rr - 0.5) * exp(- rr ) /PI; 
  }

  double sample_rad(double x, double y) 
  { 
    rr = x*x;
    return 2 * (rr - 0.5) * exp(- rr ) /PI;   
  }

  if ( g-> type == cartesian ) 
    return wigner_generate(g, sample_xy);
  else
    return wigner_generate(g, sample_rad);

}


static dmatrix* generator_one_dmatrix
  (generator* gen, int s)
{

  dmatrix* d = dmatrix_new(s);

  dmatrix_to_zero(d);

  d-> re[1][1] = 1.0;

  return d;

}


static histogram* generator_one_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double rr;

  double sample_xy(double x, double y)  
  { 
    rr = x*x + y*y;
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


static marginal* generator_one_marginal
  (generator* gen, int n, double x_max )
{

  double xx;

  double one_wavefunction(double x, double phi)
  { 
    xx = x*x;
    return 2 * xx * exp( - xx ) / sqrt(PI); 
  }

  marginal* m = marginal_new( n );
  
  marginal_mc_proba(m, one_wavefunction, n, x_max, 2.0/(sqrt(PI) * 2.5));

  return m;

}


static void generator_one_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double xx;

  double one_wavefunction(double x, double phi)
  { 
    xx = x*x;
    return 2 * xx * exp( - xx ) / sqrt(PI); 
  }
  
  marginal_mc_proba_to_quad(quad, one_wavefunction, n, x_max, 2.0/(sqrt(PI) * 2.5));

}


EXT generator*  generator_one
  ()
{
  generator* g = generator_new(0);

  g-> wig  = generator_one_wigner;
  g-> dmat = generator_one_dmatrix;
  g-> hist = generator_one_histogram;
  g-> marg = generator_one_marginal;
  g-> quad = generator_one_quadrature;

  return g;

}


/*
            (1-e) x |1><1| + e x |0><0|
*/


static wigner* generator_one_loss_wigner
  (generator* gen, grid* g)
{

  double eta = gen-> parameters[0];
  double ate = 1.0 - eta;
  double rr;

  double sample_xy(double x, double y)  
  { 
    rr = x*x + y*y;
    return 2 * ( ate * rr + eta - 0.5) * exp(- rr ) /PI; 
  }

  double sample_rad(double x, double y) 
  { 
    rr = x*x;
    return 2 * ( ate * rr + eta - 0.5) * exp(- rr ) /PI;   
  }

  if ( g-> type == cartesian ) return wigner_generate(g, sample_xy);

  return wigner_generate(g, sample_rad);

}


static dmatrix* generator_one_loss_dmatrix
  (generator* gen, int s)
{

  double eta = gen-> parameters[0];

  dmatrix* d = dmatrix_new(s);
  dmatrix_to_zero(d);

  d-> re[0][0] = eta;
  d-> re[1][1] = 1.0 - eta;

  return d;

}


static histogram* generator_one_loss_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double eta = gen-> parameters[0];
  double ate = 1.0 - eta;
  double rr;

  double sample_xy(double x, double y)  
  { 
    rr = x*x + y*y;
    return 2 * ( ate * rr + eta - 0.5) * exp(- rr ) /PI; 
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


static marginal* generator_one_loss_marginal
  (generator* gen, int n, double x_max )
{

  double eta = gen-> parameters[0];
  double ate = 1.0 - eta;
  double xx;

  double oneloss_wavefunction(double x, double phi)
  { 
    xx = x*x;
    return (2 * ate * xx + eta ) * exp( - xx ) / sqrt(PI); 
  }

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, oneloss_wavefunction, n, x_max, 1.0/sqrt(PI));

  return m;

}


static void generator_one_loss_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double eta = gen-> parameters[0];
  double ate = 1.0 - eta;
  double xx;

  double oneloss_wavefunction(double x, double phi)
  { 
    xx = x*x;
    return (2 * ate * xx + eta ) * exp( - xx ) / sqrt(PI); 
  }

  marginal_mc_proba_to_quad(quad, oneloss_wavefunction, n, x_max, 1.0/sqrt(PI));

}


EXT generator*  generator_one_loss
  (double eta)
{

  generator* g = generator_new(1);
  g-> parameters[0] = eta;

  g-> wig  = generator_one_loss_wigner;
  g-> dmat = generator_one_loss_dmatrix;
  g-> hist = generator_one_loss_histogram;
  g-> marg = generator_one_loss_marginal;
  g-> quad = generator_one_loss_quadrature;

  return g;

}


/*
            PHOTON TELE
*/


static wigner* generator_one_tele_wigner
  (generator* gen, grid* g)
{

  double b = exp( -2*gen-> parameters[0]);
  double c = exp( -4*gen-> parameters[0]);
  double s = 1.0 + 2 * b;
  double n = 1.0 / s;
  double m = 2 * n * n * n / PI;

  double sample_xy(double x, double y)  
  { 
    double rr = x*x + y*y;
    return m * ( rr + 2 * c - 0.5 ) * exp( -n * rr ); 
  }

  double sample_rad(double x, double y) 
  { 
    double rr = x*x;
    return m * ( rr + 2 * c - 0.5 ) * exp( -n * rr );   
  }

  if ( g-> type == cartesian ) return wigner_generate(g, sample_xy);

  return wigner_generate(g, sample_rad);

}


static dmatrix* generator_one_tele_dmatrix
  (generator* gen, int s)
{

  double eta = gen-> parameters[0];

  dmatrix* d = dmatrix_new(s);
  dmatrix_to_zero(d);

  d-> re[0][0] = eta;
  d-> re[1][1] = 1.0 - eta;

  return d;

}


static histogram* generator_one_tele_histogram
  (generator* gen, grid* g, range* rg)
{

  if (g->type != histo) grid_hist(g);

  double b = exp( -2*gen-> parameters[0]);
  double c = exp( -4*gen-> parameters[0]);
  double s = 1.0 + 2 * b;
  double n = 1.0 / s;
  double m = 2 * n * n * n / PI;

  double sample_xy(double x, double y)  
  { 
    double rr = x*x + y*y;
    return m * ( rr + 2 * c - 0.5 ) * exp( -n * rr ); 
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


static marginal* generator_one_tele_marginal
  (generator* gen, int n, double x_max )
{

  double b  = gen-> parameters[1];
  double s  = gen-> parameters[2];
  double t  = gen-> parameters[3];
  double nr = gen-> parameters[4];
  double y_max = gen-> parameters[5];


  double one_tele_wavefunction(double x, double phi)
  { 
    double rr = x*x;
    return nr * ( rr + b*s ) * exp( - t * rr ); 
  }

  marginal* m = marginal_new( n );

  marginal_mc_proba(m, one_tele_wavefunction, n, x_max, y_max); //x_max, 1.0);

  return m;

}


static void generator_one_tele_quadrature
  (generator* gen, double* quad, int n, double x_max )
{

  double b  = gen-> parameters[1];
  double s  = gen-> parameters[2];
  double t  = gen-> parameters[3];
  double nr = gen-> parameters[4];
  double y_max = gen-> parameters[5];

  double one_tele_wavefunction(double x, double phi)
  { 
    double rr = x*x;
    return nr * ( rr + b*s ) * exp( - t * rr ); 
  }

  marginal_mc_proba_to_quad(quad, one_tele_wavefunction, n, x_max, y_max);

}


EXT generator*  generator_one_tele
  (double r)
{

  double b = exp( -2*r);

  generator* g = generator_new(6);
  g-> parameters[0] = r;
  g-> parameters[1] = b;
  g-> parameters[2] = (1.0 + 2 * b);
  g-> parameters[3] = 1.0 / (1.0 + 2 * b);
  g-> parameters[4] = 2 * pow(1.0 + 2 * b, -2.5) / sqrt( PI );
  g-> parameters[5] = 2 * pow(1.0 + 2 * b, -1.5) * (2 + b) * exp(-(2/3)*(1.0-b)) / sqrt( PI );

  g-> wig  = generator_one_tele_wigner;
  g-> dmat = generator_one_tele_dmatrix;
  g-> hist = generator_one_tele_histogram;
  g-> marg = generator_one_tele_marginal;
  g-> quad = generator_one_tele_quadrature;

  return g;

}


