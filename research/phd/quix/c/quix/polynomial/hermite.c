

#include "quix/polynomial/hermite.h"


EXT hermite_wk* hermite_new
  (int n, range* r)
{

  int i, Nx = r->n_val;

  hermite_wk* h = quix_alloc(hermite_wk,1);

  h-> N  = n;
  h-> rg = r; 
  h-> polynomials = quix_alloc(double*,n+1);
  h-> harmonics   = quix_alloc(double*,n+1);

  loop(i,n+1) {
    h-> polynomials[i] = double_alloc( Nx ); 
    h-> harmonics[i]   = double_alloc( Nx ); 
  }

  return h;

}


EXT void hermite_free
  (hermite_wk* h)
{

  int i, m = h-> N + 1;

  loop(i,m) {
    free_if( h-> polynomials[i] );     
    free_if( h-> harmonics[i]   );
  }

  free_if( h-> polynomials ); 
  free_if( h-> harmonics );

}


EXT void hermite_calculate
  (hermite_wk* h)
{

  int x, n, N = h->N, Nx = h-> rg-> n_val;
  double  *xr   = h-> rg-> val,
         **poly = h-> polynomials, 
         **harm = h-> harmonics; 

  //assign H0 & H1

  loop(x, Nx) {
    poly[0][x] = 1.0;               
    poly[1][x] = 2*xr[x];
  }

  //calculate Hn with formula Hn+1(x) = 2xHn(x) -2nHn-1(x)
  loop(n,N)
    loop(x, Nx)    
      poly[n+1][x] = 2*( xr[x] * poly[n][x] - n * poly[n-1][x]); 
  
  double norm = 1.0/sqrt(sqrt(PI));   

  loop(n,N) {
    loop(x, Nx)    
      harm[n][x] = norm * poly[n][x] * exp( -0.5 * xr[x]*xr[x] );    
    norm /= sqrt( 2.0*(n+1) );
  }

}


