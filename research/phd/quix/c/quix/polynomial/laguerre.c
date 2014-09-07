#include "quix/polynomial/laguerre.h"


EXT laguerre_wk* laguerre_new
  (int n, range* r)
{

  laguerre_wk* l = quix_alloc(laguerre_wk,1);

  l-> N  = n;
  l-> rg = r; 
  l-> polynomials = quix_alloc(double**, n+1);

  int i, j, Nx = r-> n_val;
  loop(i,n+1)
  {
    l-> polynomials[i] = quix_alloc(double*, n+1-i);     
    loop(j, n + 1 - i)
      l-> polynomials[i][j] = double_alloc( Nx ); 
  }

  return l;

}


EXT void laguerre_free
  (laguerre_wk* l)
{

  double*** poly = l-> polynomials;

  int i,j, n = l-> N;

  loop(i, n+1) {
    loop(j, n+1)
        free_if( poly[i][j] );
      free_if( poly[i] );     
  }

  free_if( poly );

}


EXT void laguerre_calculate
  (laguerre_wk* l)
{

  int i,j,n, 
      N = l-> N, 
      Nx = l-> rg-> n_val;

  double    *x    = l-> rg-> val,
          ***poly = l-> polynomials;

  //assign L00 & L10
  loop(n,Nx) {
    poly[0][0][n] = 1.0;               
    poly[1][0][n] = 1.0 - x[n];
  }

  
  //calculate L0y with formula Lx,y+1 = sum(i,0,x) Li,y 
  for(i = 1; i < N + 1; i++)  
    loop(n, Nx)               
      poly[0][i][n] = 1.0;   
  

  //calculate Lx0 with formula Lx,0(a)=( (2*x-1-a)*Lx-1,0(a) - (x-1)*Lx-2,0(a) )/x
  for(i = 2; i < N + 1; i++)
    loop(n,Nx)
      poly[i][0][n] = ( (2*i - 1 - x[n]) * poly[i-1][0][n] - (i - 1) * poly[i-2][0][n] ) / i;  


  //calculate Lxy with formula Lx,y+1(a)=( (x+y)*Lx-1,y(a) - (x-a)*Lx,y(a) )/a
  //Lxy(0) is given by (x+y)!/(x)!(y)!
  for(i = 1; i < N; i++)
    loop(j, N-i)
      loop(n, Nx)
        if ( likely(x[n] != 0.0) )
          poly[i][j+1][n] = ( (i + j) * poly[i-1][j][n] - (i - x[n]) * poly[i][j][n] ) / x[n];
        else 
          poly[i][j+1][n] = poly[i][j][n] * (i + j + 1) / (j + 1);

}


