#include "irt.h"


#define   EPS     1.1929093e-12


static void lup_alloc
  (int size, Real* angle, Real* cos_lup, Real* sin_lup)
{

  int i;

  for (i = 0; i < size; i++) {

    cos_lup[i] = cos(angle[i]); 
    sin_lup[i] = sin(angle[i]);

  }

}


EXT wigner* irt_histogram_fast
  (histogram* h, Real cutoff, grid* g)
{

  #define A   262140//65535
  #define S   25.0
  #define R   (S/A)
  #define TO_INT(x)    round(x)

  /*
      local variables declaration
  */

  int a, b, i, j, k;
  int a_n =  h-> gd-> x-> n_val, b_n =  h-> gd-> y-> n_val;
  int x_n = g-> x-> n_val, y_n = g-> y-> n_val;

  Real c, p, z, hval, cc, ss;

  Real* x_buf = (Real*) malloc( x_n * sizeof(Real) );
  for(i = 0; i < x_n; i++ )  x_buf[i] = g-> x-> val[i];

  Real* y_buf = (Real*) malloc( y_n * sizeof(Real) );
  for(i = 0; i < y_n; i++ )  y_buf[i] = g-> y-> val[i];

  Real* ker_lup = (Real*) malloc( sizeof(Real)*A );
  ker_lup[0] = (cutoff*cutoff)/2.0;
  p = 0;
  for (i = 1; i < A; i++) {
    p += R;
    ker_lup[i] = ( cos(cutoff*p) + (cutoff*p)*sin(cutoff*p) -1.0 )/(p*p); 
  }


  /*
      main loop
  */

  Real* wig_buf = (Real*) malloc( sizeof(Real) * x_n * y_n);
  for(i = 0; i < x_n * y_n; i++ ) wig_buf[i] = 0.0;
                                  
  if (g->type == radial) {  

    Real* lup_grid_cos = (Real*) malloc( sizeof(Real) * y_n );
    Real* lup_grid_sin = (Real*) malloc( sizeof(Real) * y_n );               
    for (i = 0; i < y_n; i++) {
      lup_grid_cos[i] = cos(g-> y-> val[i]); 
      lup_grid_sin[i] = sin(g-> y-> val[i]);
    }

    a = 0;               
    while( a_n - a) {   

      cc = cos(h-> gd-> x-> val[a]) / R;
      ss = sin(h-> gd-> x-> val[a]) / R;
  
      b = 0;     
      while( b_n - b) { 

        hval = h-> val[a][b];

        if (hval != 0.0) {

          z = h-> gd-> y-> val[b] / R;

          j = 0;
          k = 0;
          while(x_n - j) { 
           
            c = lup_grid_cos[j] * cc + lup_grid_sin[j] * ss;

            i = 0;
            while(y_n - i) {

              p  = x_buf[i] * c - z ;

              wig_buf[k] += hval * ker_lup[(int) TO_INT( abs(p) )];

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
    while( a_n - a) {     

      cc = cos(h-> gd-> x-> val[a]) / R;
      ss = sin(h-> gd-> x-> val[a]) / R;
             
      b = 0;     
      while( b_n - b) { 

        hval = h-> val[a][b];

        if ( hval != 0.0 ) {

          z = h-> gd-> y-> val[b] / R;

          j = 0;
          k = 0;
          while(x_n - j) {

            c = y_buf[j] * ss - z;

            i = 0;
            while(y_n - i) { 
           
              p = c + x_buf[i] * cc ;

              wig_buf[k]     += hval * ker_lup[(int) TO_INT( abs(p) )];

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


  /*
      copy buffer to wigner struct and normalize   
  */

  wigner* w = wigner_alloc(g);

  c = (h-> gd-> x-> resolution) * (h-> gd-> y-> resolution) / (2*PI*PI);

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
  free(ker_lup);
  free(x_buf);
  free(y_buf);
  free(wig_buf);

  return w;

  #undef  TO_INT
  #undef  A
  #undef  S
  #undef  R
  #undef  KERNEL

}


