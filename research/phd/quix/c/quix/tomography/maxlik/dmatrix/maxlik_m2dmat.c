

#include "quix/tomography/maxlik/maxlik_dmatrix.h"


#define dmatrix_loop(index_i,index_j,stop)        loop(index_i,stop)\
                                                    loop(index_j,stop)


EXT void maxlik_m2dmat_single
(maxlik_dmatrix_wk* wk, dmatrix* state)
{

  marginal* margi = (marginal*) wk-> data;

  int i, j, k, l, iter,
      J = wk-> iterations,
      N = wk-> photons, 
      M = wk-> photons + 1,
      K = margi-> samples;

  double  norm  = 1.0 / (K),
         *quad  = margi-> quad,
         *h_lup = double_alloc(M),
         *c_lup = double_alloc(K),
         *s_lup = double_alloc(K);


  loop(k,K) {
    c_lup[k] = cos(margi-> phase[k]);
    s_lup[k] = sin(margi-> phase[k]);
  }

  dmatrix *operator = dmatrix_new(N),
          *prod     = dmatrix_new(N),
          *proj     = dmatrix_new(N),
          *last     = dmatrix_new(N);

  //double *proj_re = double_alloc(M*M),
  //       *proj_im = double_alloc(M*M); 

  loop(iter,J) {

    loop(i,M) {
      loop(j,M) {
        last->re[i][j] = state->re[i][j];
        last->im[i][j] = state->im[i][j];
      }
    }

    dmatrix_to_zero(operator);

    loop(k,K) { 

      double x = quad[k];
      
      h_lup[0] = exp( - x * x / 2 ) / sqrt( sqrt(PI) );
      h_lup[1] = sqrt(2) * x * h_lup[0];

      i = 2;
      while(M - i)
        h_lup[i++] = sqrt(2.0/i) * x * h_lup[i-1] - sqrt((i-1)*1.0/i)*h_lup[i-2];   

      double  c   = c_lup[k],
              s   = s_lup[k],
              c_i = 1.0,
              s_i = 0.0,
              trc = 0.0;

      //l = 0;
      loop(i,M) {

        double  c_ij = c_i,
                s_ij = s_i;

        loop(j,M) {

          x = h_lup[i]*h_lup[j];
          double  a = c_ij * x,
                  b = s_ij * x;

          proj-> re[i][j] = a; 
          proj-> im[i][j] = b;  

//          proj_re[l] = a;
//          proj_im[l] = b;
//l++;
          trc += a * state-> re[i][j] - b * state-> im[i][j];  

          x = c_ij;
          c_ij  = s * s_ij + c * x; 
          s_ij  = c * s_ij - s * x;

        }

        x  = c_i;
        c_i = c * x - s * s_i;
        s_i = s * x + c * s_i;

      }

      trc = norm / trc;

//l=0;
      loop(i,M) {
        loop(j,M) {
          operator-> re[i][j] += trc * proj-> re[i][j]; 
          operator-> im[i][j] += trc * proj-> im[i][j]; 
          //operator-> re[i][j] += trc * proj_re[l]; 
          //operator-> im[i][j] += trc * proj_im[l]; 
//l++;
        }
      }

    }

    /* quicker to copy in operator in memory order and transpose at the end */

    dmatrix_transpose(operator);             
    dmatrix_prod(prod, operator, state);
    dmatrix_prod(state, prod, operator);
    dmatrix_renorm(state); 

    wk->distance[iter] = dmatrix_dist_frob(state, last);

    if(wk-> callback != NULL) 
      (wk-> callback)(state);

    if(wk-> report) 
      if( iter % wk-> report == 0) 
        printf("%i iterations done\n",iter+1);
    
  }

  dmatrix_free(operator);
  dmatrix_free(prod);
  dmatrix_free(proj);
  dmatrix_free(last);

  free(c_lup);
  free(s_lup);
  free(h_lup);

//free(proj_re);
//free(proj_im);

}


EXT void maxlik_m2dmat_multi
(maxlik_dmatrix_wk* wk, dmatrix* state, int nthread)
{

  marginal* margi = (marginal*) wk-> data;

  int a, i, j, k, iter,
      report  = wk-> report,
      J       = wk-> iterations,
      N       = wk-> photons, 
      M       = wk-> photons + 1,
      K       = margi-> samples,
      offset  = margi-> samples / nthread;

  double    norm   = 1.0 / (K),
           *c_lup  = double_alloc(K),
           *s_lup  = double_alloc(K),
          **h_lups = quix_alloc(double*,nthread);

  dmatrix  *operator  = dmatrix_new(N),
           *prod      = dmatrix_new(N),
           *last      = dmatrix_new(N);
          //**projs     = quix_alloc(dmatrix*,nthread),
          //**opers     = quix_alloc(dmatrix*,nthread);
  double  **projs_re  = quix_alloc(double*,nthread),
          **projs_im  = quix_alloc(double*,nthread),
          **opers_re  = quix_alloc(double*,nthread),
          **opers_im  = quix_alloc(double*,nthread);

  loop(k,K) {
    c_lup[k] = cos(margi-> phase[k]);
    s_lup[k] = sin(margi-> phase[k]);
  }

  loop(a,nthread) {
    h_lups[a] = double_alloc(M);
    //projs[a]  = dmatrix_new(N);
    //opers[a]  = dmatrix_new(N);
    projs_re[a]  = double_alloc(M*M);
    projs_im[a]  = double_alloc(M*M);
    opers_re[a]  = double_alloc(M*M);
    opers_im[a]  = double_alloc(M*M);
  }


  void* compute_partial_projector(void* arg) 
  {

    int ii, jj, kk, l, job = (int) arg;

    //dmatrix  *proj = projs[job],
    //         *oper = opers[job];
    double   *proj_re = projs_re[job],
             *proj_im = projs_im[job],
             *oper_re = opers_re[job],
             *oper_im = opers_im[job];

    double   *quad = margi-> quad + job * offset,
             *clup = c_lup        + job * offset,
             *slup = s_lup        + job * offset,
             *hlup = h_lups[job],
            **s_re = state-> re,
            **s_im = state-> im;

    //dmatrix_to_zero(oper);    
    loop(l,M*M) { oper_re[l] = 0.0; oper_im[l] = 0.0; }

    loop(kk,offset) { 

      double x = quad[kk];
      
      hlup[0] = exp( - x * x / 2 ) / sqrt( sqrt(PI) );
      hlup[1] = sqrt(2) * x * hlup[0];

      l = 2;
      while(M - l)
        hlup[l++] = sqrt(2.0/l) * x * hlup[l-1] - sqrt((l-1)*1.0/l)*hlup[l-2];   

      double  c   = clup[kk],
              s   = slup[kk],
              c_i = 1.0,
              s_i = 0.0,
              trc = 0.0;

      l = 0;
      loop(ii,M) 
      {
        double  c_ij = c_i,
                s_ij = s_i;
        loop(jj,M) 
        {
          x = hlup[ii]*hlup[jj];
          double  xc = c_ij * x,
                  xs = s_ij * x;
          //proj-> re[ii][jj] = xc; 
          //proj-> im[ii][jj] = xs;  
          proj_re[l] = xc; 
          proj_im[l] = xs; 
          trc  += xc * s_re[ii][jj] - xs * s_im[ii][jj];  
          x     = c_ij;
          c_ij  = s * s_ij + c * x; 
          s_ij  = c * s_ij - s * x;
l++;
        }
        x   = c_i;
        c_i = c * x - s * s_i;
        s_i = s * x + c * s_i;
      }

      trc = norm / trc;
/*
      loop(ii,M) {
        loop(jj,M) {
          oper-> re[ii][jj] += trc * proj-> re[ii][jj]; 
          /per-> im[ii][jj] += trc * proj-> im[ii][jj];
        } 
      }
*/
      loop(l,M*M) {
        oper_re[l] += trc * proj_re[l]; 
        oper_im[l] += trc * proj_im[l];
      }

    }

    job_finish();

  }


  loop(iter,J) {

    loop(i,M) {
      loop(j,M) {
        last->re[i][j] = state->re[i][j];
        last->im[i][j] = state->im[i][j];
      }
    }

    loop(a,nthread) job_start(compute_partial_projector, (void*)a);
    job_wait_all();

int z = 0;
    dmatrix_to_zero(operator);
    loop(i,M) {
      loop(j,M) {
        loop(a,nthread) {
          //operator->re[i][j] += opers[a]->re[i][j];
          //operator->im[i][j] += opers[a]->im[i][j];
          operator->re[i][j] += opers_re[a][z];
          operator->im[i][j] += opers_im[a][z];
        }
z++;
      }
    }

    dmatrix_transpose(operator);             
    dmatrix_prod(prod, operator, state);
    dmatrix_prod(state, prod, operator);
    dmatrix_renorm(state); 

    //wk->distance[iter] = dmatrix_dist_frob(state, last);

    //if(wk-> callback != NULL)   (wk-> callback)(state);

    if(report) if( iter % report == 0)  printf("%i iterations done\n",iter+1);
    
  }


  loop(a,nthread)               free(h_lups[a]);
  //loop(a,nthread)               dmatrix_free( opers[a] );
  //loop(a,nthread)               dmatrix_free( projs[a] );
  loop(a,nthread)               free( opers_re[a] );
  loop(a,nthread)               free( opers_im[a] );
  loop(a,nthread)               free( projs_re[a] );
  loop(a,nthread)               free( projs_im[a] );

  free(h_lups);                 h_lups    = NULL;
  //free(opers);                  opers     = NULL;
  //free(projs);                  projs     = NULL;
  free(opers_re);             
  free(opers_im);                 
  free(projs_re);              
  free(projs_im);                


  free(c_lup);                  c_lup     = NULL;
  free(s_lup);                  s_lup     = NULL;
  dmatrix_free( operator );     operator  = NULL;
  dmatrix_free( prod );         prod      = NULL;
  dmatrix_free( last );         last      = NULL;


}


#undef dmatrix_loop


EXT void maxlik_m2diag
(maxlik_dmatrix_wk* wk, double* diag)
{

  marginal* margi = (marginal*) wk-> data;

  int i, j, k, l, iter,
      J = wk-> iterations,
      N = wk-> photons, 
      M = wk-> photons + 1,
      K = margi-> samples;

  double  norm  = 1.0 / (K),
         *quad  = margi-> quad,
         *oper  = double_alloc(M),
         *proj  = double_alloc(M),
         *lup1  = double_alloc(M),
         *lup2  = double_alloc(M);

  loop(i,M) {
    diag[i] = 1.0 / M;
    lup1[i] = sqrt(2.0/i);
    lup2[i] = sqrt((i-1)*1.0/i);
  }

  loop(iter,J) {

    loop(i,M)
      oper[i] = 0.0;

    loop(k,K) { 

      double  x   = quad[k];

      proj[0] = 1.0;
      proj[1] = 2 * x * x;
      
      double trc =  diag[0] + diag[1] * proj[1],
              h1 = 1.0,
              h2 = sqrt(2) * x,
              h3 = 0.0;

      i = 2;
      while(M - i){
        h3 = lup1[i] * x * h2 - lup2[i] * h1;   
        h1 = h2;
        h2 = h3;
        proj[i] = h3*h3; 
        trc += diag[i] * proj[i];         
        i++;
      }

      trc = norm / trc;
      loop(i,M)
        oper[i] += trc * proj[i]; 

    }

    double diag_trc = 0.0;
    loop(i,M) {
      diag[i]  *= oper[i];
      diag_trc += diag[i];
    }
    diag_trc = 1.0/diag_trc;
    loop(i,M) 
      diag[i] *= diag_trc;

    /* implement frob dist for diag elements only */
    //wk->distance[iter] = dmatrix_dist_frob(state, last);

    //if(wk-> callback != NULL) 
    //  (wk-> callback)((dmatrix*)diag);

    if(wk-> report) 
      if( iter % wk-> report == 0) 
        printf("%i diag iterations done\n",iter+1);
    
  }

  free(oper);
  free(proj);
  free(lup1);
  free(lup2);

}


