

#include "quix/tomography/irt/irt_mc_variance.h"
#include "quix/tomography/irt/irt_kernel.h"


EXT double* kirt_mc_h2neg_var    
  (int n, double cutoff, get_histogram callback) 
{

  int i;

  double neg, nrm, avrg = 0.0, disp = 0.0, buff = 0.0;
  double* dispersion = double_alloc(n);

  loop(i,n) {
  
    histogram* h = callback();
    kirt_h2neg(h,cutoff, &neg);

    disp += neg * neg;
    avrg += neg;

    nrm = 1.0 / (i+1);
    dispersion[i] = sqrt(  nrm * ( disp - nrm * avrg*avrg ) );

    histogram_free(h);

  }

  return dispersion;

}


EXT double* kirt_mc_m2neg_var     
  (int n, double cutoff, get_marginal callback) 
{

  int i;

  double neg, nrm, avrg = 0.0, disp = 0.0, buff = 0.0;
  double* dispersion = double_alloc(n);

  loop(i,n) {
  
    marginal* m = callback();
    kirt_m2neg( m, cutoff, &neg);

    disp += neg * neg;
    avrg += neg;

    nrm = 1.0 / (i+1);
    dispersion[i] = sqrt(  nrm * ( disp - nrm * avrg*avrg ) );

    marginal_free(m);

  }

  return dispersion;

}


EXT double* kirt_mc_g2neg_var
  (int n, double cutoff, generator* gen, int l, double x_max)
{

  int i;

  double neg, nrm, avrg = 0.0, disp = 0.0, buff = 0.0;
  double* dispersion = double_alloc(n);
  double* quadrature = double_alloc(l);

  marginal* m = marginal_new(l);

  loop(i,n) {
  
    gen-> quad(gen, quadrature, l, x_max);
    m-> quad = quadrature;
    kirt_m2neg( m, cutoff, &neg);

    disp += neg * neg;
    avrg += neg;

    nrm = 1.0 / (i+1);    
    dispersion[i] = sqrt(  nrm * ( disp - nrm * avrg*avrg ) );

  }

  free(quadrature);

  return dispersion;

}


EXT negvar_wk* kirt_mc_m2negvar_wk
  (int n, double cutoff, get_marginal callback) 
{

  negvar_wk* ng = irt_negvar_new(n); 

  int i;

  double neg, var, nrm, avrg = 0.0, disp = 0.0, buff = 0.0;

  loop(i,n) {
  
    marginal* m = callback();
    kirt_m2neg_var( m, cutoff, &neg, &var);

    disp += neg * neg;
    avrg += neg;

    nrm = 1.0 / (i+1);    
    ng-> dispersion[i] = sqrt(  nrm * ( disp - nrm * avrg*avrg ) );
    ng-> negativity[i] = neg;
    ng-> variance[i]   = var;

    marginal_free(m);

  }

  return ng;

}


EXT negvar_wk* kirt_mc_g2negvar_wk
  (int n, double cutoff, generator* gen, int l, double x_max)
{

  negvar_wk* ng = irt_negvar_new(n); 

  int i;

  double neg, var, nrm, avrg = 0.0, disp = 0.0, buff = 0.0;
  double* quadrature = double_alloc(l);

  marginal* m = marginal_new(l);

  loop(i,n) {
  
    gen-> quad(gen, quadrature, l, x_max);
    m-> quad = quadrature;
    kirt_m2neg_var( m, cutoff, &neg, &var);

    disp += neg * neg;
    avrg += neg;
  
    nrm = 1.0 / (i+1);  
    ng-> dispersion[i] = sqrt(  nrm * ( disp - nrm * avrg*avrg ) );
    ng-> negativity[i] = neg;
    ng-> variance[i]   = var;

  }

  free(quadrature);

  return ng;

}


