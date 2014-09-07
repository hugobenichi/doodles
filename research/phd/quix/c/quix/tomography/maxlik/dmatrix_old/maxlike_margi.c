#include "maxlike.h"


EXT maxlike_margi_wk* maxlike_margi_alloc
  (int N, marginal* m)
{

  maxlike_margi_wk* max = (maxlike_margi_wk*) malloc(sizeof(maxlike_margi_wk));

  max-> print     = 0;
  max-> iteration = 0;
  max-> data      = m;
  max-> state     = dmatrix_alloc(N);
  max-> tester    = NULL;

  dmatrix_to_Id(max-> state);

  return max;

}


EXT void maxlike_margi_free
  (maxlike_margi_wk* max)
{

  max-> data = NULL;
  free( max-> state );
  free( max );

}


EXT dmatrix* maxlike_margi_state
  (maxlike_margi_wk* max)
{

  return dmatrix_copy(max->state);

}


EXT Real* maxlike_margi_iteration
  (maxlike_margi_wk* max, int J)
{

  int k = 0, i = 0, j = 0, l;
  int K = max-> data-> samples, M = max-> state-> n + 1;

  Real accum, trace, norm = 1.0 / (max-> data-> samples);
  Real c, c_i, c_ij, s, s_i, s_ij, a, b;

  Real* h_lup = (Real*) malloc( sizeof(Real) * M);
  Real* c_lup = (Real*) malloc( sizeof(Real) * K);
  Real* s_lup = (Real*) malloc( sizeof(Real) * K);

  for(k = 0; k < K; k++) {
    c_lup[k] = cos(max-> data-> phase[k]);
    s_lup[k] = sin(max-> data-> phase[k]);
  }

  dmatrix* operator = dmatrix_alloc(max->state->n);
  dmatrix* prod     = dmatrix_alloc(max->state->n);
  dmatrix* proj     = dmatrix_alloc(max->state->n);
  dmatrix* last     = dmatrix_alloc(max->state->n);
  dmatrix* state    = max-> state;

  Real* err = (Real*) malloc( J * sizeof(Real) );

  int iter = max-> iteration;
  max-> iteration += J;
  J = max-> iteration;

  while(J - iter) {

    i = 0;
    while(M - i) {
      j = 0;
      while(M - j) {
        last->re[i][j] = state->re[i][j];
        last->im[i][j] = state->im[i][j];
        j++;
      }
      i++;
    }  

    dmatrix_to_zero(operator);

    k = 0;
    while(K - k) { 

      accum = max-> data-> quad[k];
      
      h_lup[0] = exp( - accum * accum / 2 ) / sqrt( sqrt(PI) );
      h_lup[1] = sqrt(2) * accum * h_lup[0];

      i = 2;
      while(M - i) {

        h_lup[i] = sqrt(2.0/i) * accum * h_lup[i-1] - sqrt((i-1)*1.0/i)*h_lup[i-2];   

        i++;
      }

      c = c_lup[k];
      s = s_lup[k];

      c_i  = 1.0;
      s_i  = 0.0;

      trace = 0.0;

      i = 0;
      while(M - i) {

        c_ij = c_i;
        s_ij = s_i;

        j = 0;
        while(M - j) {

          accum = h_lup[i]*h_lup[j];

          a = c_ij * accum;
          b = s_ij * accum;

          proj-> re[i][j] = a; 
          proj-> im[i][j] = b;  

          trace += a * state-> re[i][j] - b * state-> im[i][j];  

          accum = c_ij;
          c_ij  = s * s_ij + c * accum; 
          s_ij  = c * s_ij - s * accum;

          j++;
        }

        accum  = c_i;

        c_i = c * accum - s * s_i;
        s_i = s * accum + c * s_i;

        i++;
      }

      trace = norm / trace;

      i = 0;
      while(M - i) {

        j = 0;
        while(M - j) {

          operator-> re[i][j] += trace * proj-> re[i][j]; 
          operator-> im[i][j] += trace * proj-> im[i][j]; 

          j++;
        }

        i++;
      }

      k++;
    }

    /* quicker to copy in operator in memory order and transpose at the end */

    dmatrix_transpose(operator);             
    dmatrix_prod(prod, operator, state);
    dmatrix_prod(state, prod, operator);
    dmatrix_renorm(state); 

    err[k] = dmatrix_dist_max(state, last);
    k++;  

    if(max-> print) 
      printf("err(%i):%f\n",iter,err[k]);

    if(max-> tester != NULL) 
      (max-> tester)(iter,state);

    iter++;
    if( iter % 10 == 0) 
      printf("%i iterations done\n",iter);
    
  }

  dmatrix_free(operator);
  dmatrix_free(prod);
  dmatrix_free(proj);
  dmatrix_free(last);

  free(c_lup);
  free(s_lup);
  free(h_lup);

  return err;

}


