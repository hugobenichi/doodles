

#include "quix/tomography/irt/irt_kernel.h"


EXT void kirt_m2xy
  (marginal* m, double cutoff, grid* g, wigner* w, int nthread)
{

  #define   A             262140//65535
  #define   S             25.0
  #define   R             (S/A)
  #define   TO_INT(x)     round(x)

  int a, i, j, k, 
      offset = m-> samples / nthread,
      x_n = g-> x-> n_val, 
      y_n = g-> y-> n_val, 
      M   = m-> samples;

  double  c, p, pp, ppp, z, cc, ss,
         *x_buf = double_alloc( x_n ),
         *y_buf = double_alloc( y_n ),
         *k_lup = double_alloc( A ),
        **phase = quix_alloc(double*, nthread),
        **quad  = quix_alloc(double*, nthread),
        **w_buf = quix_alloc(double*, nthread);

  loop(i,x_n)  x_buf[i] = g-> x-> val[i];
  loop(i,y_n)  y_buf[i] = g-> y-> val[i];


  k_lup[0] = (cutoff*cutoff)/2.0;
  p = 0.0;
  for (i = 1; i < A; i++) {
    p += R;
    k_lup[i] = ( cos(cutoff*p) + (cutoff*p)*sin(cutoff*p) - 1.0 )/(p*p); 
  }


  loop(a,nthread) {
    phase[a] = m-> phase + offset*a;
    quad[a]  = m-> quad  + offset*a;
    w_buf[a] = double_alloc( x_n * y_n);
  }

  
  void* partial_kirt_xy(void* arg) 
  {

    int x, y, u, s, 
        xn  = x_n, 
        yn  = y_n, 
        stp = offset, 
        job = (int) arg; 

    double  *xb = x_buf,
            *yb = y_buf,   
            *kl = k_lup,      
            *qd = quad[job],
            *pz = phase[job], 
            *wb = w_buf[job];

    loop(u, x_n*y_n) wb[u] = 0.0;            

    loop(s, stp) {     
      int u  = 0;                                                
      double  z  = qd[s] / R,
              cc = cos( pz[s] ) / R,
              ss = sin( pz[s] ) / R;
      loop(x, xn)  {                                     
        double c = xb[x] * cc - z;
        loop(y, yn)
          wb[u++] += kl[(int)TO_INT( fabs(c + yb[y] * ss) )]; 
      }
    } 

    job_finish();
  }

  loop(a,nthread) job_start(partial_kirt_xy, (void*)a);
  job_wait_all();

  c = 1.0 / ( 2 * PI * M );
  k = 0;
  
  loop(i,x_n)
    loop(j,y_n) {
      loop(a,nthread)
        w-> val[i][j] += w_buf[a][k] * c;
      k++;
    }


  free(x_buf);
  free(y_buf);
  free(k_lup);
  loop(a, nthread)  
    free(w_buf[a]);
  free(w_buf);
  free(phase);
  free(quad);

  #undef  A
  #undef  S
  #undef  R
  #undef  TO_INT

}


EXT void kirt_m2xy_single
  (marginal* m, double cutoff, grid* g, wigner* w)
{

  #define   A             262140//65535
  #define   S             25.0
  #define   R             (S/A)
  #define   TO_INT(x)     round(x)


  int a, i, j, k,
      x_n = g-> x-> n_val, 
      y_n = g-> y-> n_val, 
      M   = m-> samples;

  double  c, p, pp, ppp, z, cc, ss,
         *x_buf = double_alloc( x_n ),
         *y_buf = double_alloc( y_n ),
         *w_buf = double_alloc( x_n * y_n),
         *k_lup = double_alloc( A );

  loop(i, x_n)     x_buf[i] = g-> x-> val[i];
  loop(i, y_n)     y_buf[i] = g-> y-> val[i];
  loop(i, x_n*y_n) w_buf[i] = 0.0;

  k_lup[0] = (cutoff*cutoff)/2.0;
  p = 0.0;
  for (i = 1; i < A; i++) {
    p += R;
    k_lup[i] = ( cos(cutoff*p) + (cutoff*p)*sin(cutoff*p) - 1.0 )/(p*p); 
  }


  loop(a,M) {     

    k  = 0;                                                
    z  = m-> quad[a] / R;
    cc = cos( m-> phase[a] ) / R;
    ss = sin( m-> phase[a] ) / R;

    loop(j,y_n)  {                 
                   
      c = y_buf[j] * ss - z;

      loop(i,x_n) 
        w_buf[k++] += k_lup[(int)TO_INT( fabs(c + x_buf[i] * cc) )]; 

    }

  }


  c = 1.0 / ( 2 * PI * M );
  k = 0;
  
  loop(j,y_n)
    loop(i,x_n)
      w-> val[i][j] = w_buf[k++] * c;


  free(x_buf);
  free(y_buf);
  free(w_buf);
  free(k_lup);


  #undef  TO_INT
  #undef  A
  #undef  S
  #undef  R

}


