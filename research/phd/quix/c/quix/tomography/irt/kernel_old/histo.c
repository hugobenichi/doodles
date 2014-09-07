#include "irt.h"


#define   EPS     1.1929093e-12


EXT wigner* irt_histogram
  (histogram* h, Real cutoff, grid* g)
{

  int a, b, i, j, k;
  int a_n =  h-> gd-> x-> n_val, b_n =  h-> gd-> y-> n_val;
  int x_n = g-> x-> n_val, y_n = g-> y-> n_val;

  Real c, p, pp, hval, z, cc, ss, sqr = cutoff*cutoff, sqr2 = sqr/2;

  Real* x_buf = (Real*) malloc( x_n * sizeof(Real) );
  for(i = 0; i < x_n; i++ )  x_buf[i] = g-> x-> val[i];

  Real* y_buf = (Real*) malloc( y_n * sizeof(Real) );
  for(i = 0; i < y_n; i++ )  y_buf[i] = g-> y-> val[i];
   
                           
  /*
      main loop
  */

  Real* wig_buf = (Real*) malloc( sizeof(Real) * x_n * y_n);
  for(i = 0; i < x_n * y_n; i++ ) wig_buf[i] = 0.0;
                                  
  if (g->type == radial) {

    Real* lup_grid_cos = (Real*) malloc( sizeof(Real) * y_n );
    Real* lup_grid_sin = (Real*) malloc( sizeof(Real) * y_n );
    for (i = 0; i < y_n; i++) {
      lup_grid_cos[i] = cos( g-> y-> val[i]); 
      lup_grid_sin[i] = sin( g-> y-> val[i]);
    }

    a = 0;                  
    while(a_n - a) { 

      cc = cos(h-> gd-> x-> val[a]);
      ss = sin(h-> gd-> x-> val[a]);
              
      b = 0;      
      while(b_n - b) { 

        z = h-> gd-> y-> val[b];
           
        hval = h->val[a][b];
        if ( likely(hval != 0.0) ) {

          j = 0;
          k = 0;
          while(y_n - j){                 
            c = lup_grid_cos[j] * cc + lup_grid_sin[j] * ss;

            i = 0;
            while(x_n - i){

              p = cutoff * ( x_buf[i] * c - z );

              if ( unlikely( p == 0.0) ) pp = sqr2;
              else            pp = sqr*( cos(p) + p * sin(p) - 1.0 ) / (p*p);

              wig_buf[k] += hval * pp;    

              i++;
              k++;
            }

            j++; 
          }

        }

        b++;
      }

      a++;
    }

    free(lup_grid_cos);
    free(lup_grid_sin);

  } else {  
                   
    a = 0;                  
    while(a_n - a) { 

      cc = cos(h-> gd-> x-> val[a]);
      ss = sin(h-> gd-> x-> val[a]);
                
      b = 0;      
      while(b_n - b) { 

        z = h->gd->y->val[b];

        hval = h-> val[a][b];
        if ( hval != 0.0 ) {

          j = 0;
          k = 0;
          while(y_n - j){    

            c = y_buf[j] * ss - z;

            i = 0;
            while(x_n - i){

              p = cutoff * ( c + x_buf[i]*cc );

              if (unlikely( p == 0.0) ) pp = sqr2;
              else            pp = sqr*( cos(p) + p * sin(p) - 1.0 ) / (p*p);
              wig_buf[k] += hval * pp;    

              i++;
              k++;
            }

            j++;
          }

        }
    
        b++;
      }

      a++;
    }

  }

  wigner* w = wigner_alloc(g);

  c = (h->gd->x->resolution)*(h->gd->y->resolution)/(2*PI*PI); 

  k = 0;
  for (j = 0; j < y_n; j++) {
    for (i = 0; i < x_n; i++) {
      w-> val[i][j] = wig_buf[k] * c;
      k++;
    }
  }

  /*
      clean memory     
  */

  free(x_buf);
  free(y_buf);
  free(wig_buf);

  return w;

}


EXT Real irt_histogram_neg
  (histogram* h, Real cutoff)
{

  Real kernel
    (Real x) 
  {
    if ( unlikely(x == 0.0) ) 
      return cutoff*cutoff/2;
    Real t = cutoff*x;

    return (cos(t)+t*sin(t)-1)/(x*x);

  }

  Real negativity = 0.0;
  int a,b;

  Real c = 0.0;
  for (b = 0; b < h->gd->y->n_val; b++ ) { 

    c = kernel( (h->gd->y->val[b]) );

    for (a = 0; a < h->gd->x->n_val; a++)        
      negativity += (h->val[a][b])*c;

  }

  negativity *= (h->gd->x->resolution)*(h->gd->y->resolution)/(2*PI*PI); 

  return negativity;

}


#undef EPS


