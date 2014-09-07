

#include "quix/tomography/irt/irt_kernel.h"


EXT wigner*
kirt_marginal          
(marginal* m, double cutoff, grid* g, int nthread)
{

    wigner* w = wigner_new(g);
    wigner_to_zero(w);

    if(likely(nthread))
      if_cartesian(g)
        kirt_m2xy(m, cutoff, g, w, nthread);
      else
        kirt_m2rad(m, cutoff, g, w, nthread);
    else
      if_cartesian(g)
        kirt_m2xy_single(m, cutoff, g, w);
      else
        kirt_m2rad_single(m, cutoff, g, w);

    return w;

}


static double  ext_cutoff = -1.0;
static double* ext_kernel = NULL;

static void fill_kernel(double cutoff, double rez, int siz) {
  
    if (!ext_kernel)
      ext_kernel = double_alloc( siz );

    ext_cutoff = cutoff;

    ext_kernel[0] = (cutoff*cutoff)/2.0;
    int i;
    double p = rez;
    for (i = 1, p = rez; i < siz; i++, p+= rez)
      ext_kernel[i] = ( cos(cutoff*p) + cutoff*p * sin(cutoff*p) - 1.0 ) / (p*p); 

}


#define   A             65535
#define   S             10.0
#define   R             (S/A)
#define   TO_INT(x)     round(x)


EXT void kirt_m2neg
  (marginal* m, double cutoff, double* negativity)
{ 

  int i, n = m-> samples;

  double  neg  = 0.0,
          cut2 = cutoff*cutoff /2,
         *quad = m-> quad;

  loop(i,n) {
    double x = quad[i];
    if ( unlikely(x == 0.0) )
      neg += cut2;
    else {
      double y = cutoff * x;
      neg += ( cos(y) + y*sin(y) - 1.0 )/(x*x);
    }
  }

  *negativity =  neg / (2*PI*n );

}


EXT void kirt_m2neg_var
  (marginal* m, double cutoff, double* negativity, double* variance)
{ 

  if ( unlikely(ext_kernel == NULL) ) fill_kernel(cutoff, R, A);

  int i, n = m-> samples;

  double neg = 0.0,
         var = 0.0;

  loop(i,n) {
    double y = ext_kernel[(int) TO_INT( fabs( m-> quad[i] / R ) )];
    neg   += y;
    var   += y*y;
  }

  neg /= (2*PI*n );

  *negativity = neg;
  *variance   = sqrt( ( var / (4*PI*PI*n ) - neg * neg ) / (n - 1) );

}


#undef  TO_INT
#undef  A
#undef  S
#undef  R


