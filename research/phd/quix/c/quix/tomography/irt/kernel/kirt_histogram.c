

#include "quix/tomography/irt/irt_kernel.h"


EXT wigner*
kirt_histogram         
(histogram* h, double cutoff, grid* g, int nthread)
{

    wigner* w = wigner_new(g);
    wigner_to_zero(w);

    if(likely(nthread))
      if_cartesian(g)
        kirt_h2xy(h, cutoff, g, w, nthread);
      else
        kirt_h2rad(h, cutoff, g, w, nthread);
    else
      if_cartesian(g)
        kirt_h2xy_single(h, cutoff, g, w);
      else
        kirt_h2rad_single(h, cutoff, g, w);

    return w;

}


static double* ext_kernel = NULL; //need to add static cleaner

static void fill_kernel(double cutoff, double rez, int siz) {
  
    ext_kernel    = double_alloc( siz );
    ext_kernel[0] = (cutoff*cutoff)/2.0;

    int i;
    double p = rez;
    for (i = 1, p = 0.0; i < siz; i++, p+= rez)
      ext_kernel[i] = ( cos(cutoff*p) + cutoff*p * sin(cutoff*p) - 1.0 ) / (p*p); 

}


#define   A             65535
#define   S             10.0
#define   R             (S/A)
#define   TO_INT(x)     round(x)


EXT void kirt_h2neg
  (histogram* h, double cutoff, double* negativity)
{ 


}


EXT void kirt_h2neg_var
  (histogram* h, double cutoff, double* negativity, double* variance)
{ 



}


#undef  TO_INT
#undef  A
#undef  S
#undef  R


