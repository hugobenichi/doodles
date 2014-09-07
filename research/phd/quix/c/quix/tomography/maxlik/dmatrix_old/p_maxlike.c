#include "p_maxlike.h"


EXT pmaxlike_wk* pmaxlike_wkalloc
  (int a, int b, marginal* m, int N)
{

  pmaxlike_wk* wk = (pmaxlike_wk*) malloc( sizeof(pmaxlike_wk) );

  int K = b - a;
  wk-> K = K;

  wk-> oper  = dmatrix_alloc(N);
  wk-> proj  = dmatrix_alloc(N);

  wk-> quad  = (Real*) malloc( K * sizeof(Real) );
  wk-> c_lup = (Real*) malloc( K * sizeof(Real) );
  wk-> s_lup = (Real*) malloc( K * sizeof(Real) );

  int i;
  for (i = 0; i < K; i++) {
    wk-> quad[i]  = m-> quad[i + a];
    wk-> c_lup[i] = cos( m-> phase[i + a]);
    wk-> s_lup[i] = sin( m-> phase[i + a]);
  }

  wk-> h_lup = (Real*) malloc( ( N + 1 ) * sizeof(Real) );  

  return wk;

}


EXT void pmaxlike_wkfree
  (pmaxlike_wk* wk)
{

  dmatrix_free(wk-> oper);
  dmatrix_free(wk-> proj);

  free(wk-> quad);
  free(wk-> c_lup);
  free(wk-> s_lup);
  free(wk-> h_lup);

  free(wk);

}


EXT Real* pmaxlike_margi_iteration
  (maxlike_margi_wk* max, int J)
{

  int a, i, j, k;
  int K = max-> data-> samples, N = max-> state-> n, M = N + 1;
  Real re, im;
  Real norm = 1.0 / (max-> data-> samples);

  dmatrix* operator = dmatrix_alloc(max->state->n);
  dmatrix* prod     = dmatrix_alloc(max->state->n);
  dmatrix* proj     = dmatrix_alloc(max->state->n);
  dmatrix* last     = dmatrix_alloc(max->state->n);
  dmatrix* state    = max-> state;

  Real* err = (Real*) malloc( J * sizeof(Real) );
  
  int iter = max-> iteration;
  max-> iteration += J;
  J = max-> iteration;


  void *status;
  pthread_t      threads[NUM_THREADS];
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


  pmaxlike_wk* data[NUM_THREADS];

  j = K / NUM_THREADS;
  #pragma unroll 
  for (a = 0; a < NUM_THREADS; a++) {
    data[a] = pmaxlike_wkalloc(a * j, (a +1) * j, max-> data, N);
    data[a]-> state = state;
  }

  k = 0;
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

    #pragma unroll 
    for (a = 0; a < NUM_THREADS; a++)
      pthread_create(&threads[a], &attr, pmaxlike_thread_getproj, (void *) data[a]);

    #pragma unroll 
    for (a = 0; a < NUM_THREADS; a++)
      pthread_join(threads[a], &status);

    i = 0;
    while(M - i) {
      j = 0;
      while(M - j) {

        re = 0.0;
        im = 0.0;

        #pragma unroll
        for (a = 0; a < NUM_THREADS; a++)
          re += data[a]-> oper-> re[i][j]; 
        #pragma unroll
        for (a = 0; a < NUM_THREADS; a++)
          im += data[a]-> oper-> im[i][j]; 

        operator-> re[i][j] = norm * re;
        operator-> im[i][j] = norm * im;

        j++;
      }
      i++;
    }    
    

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
      ;//printf("%i iterations done\n",iter);
    
  }


  dmatrix_free(operator);
  dmatrix_free(prod);
  dmatrix_free(proj);
  dmatrix_free(last);

  #pragma unroll 
    for (a = 0; a < NUM_THREADS; a++)
      pmaxlike_wkfree( data[a] );

  pthread_attr_destroy(&attr);

  return err;

}


static void*  pmaxlike_thread_getproj
  (void* args)
{

  pmaxlike_wk* wk = (pmaxlike_wk*) args;

  int k, i, j;
  //Real c_ij, s_ij;

  int K = wk-> K;
  int M = wk-> state-> n + 1;

  Real** restrict state_re = wk-> state-> re;
  Real** restrict state_im = wk-> state-> im;
  Real** restrict oper_re  = wk-> oper-> re;
  Real** restrict oper_im  = wk-> oper-> im;
  Real** restrict proj_re  = wk-> proj-> re;
  Real** restrict proj_im  = wk-> proj-> im;

  Real* restrict quad  = wk-> quad;
  Real* restrict c_lup = wk-> c_lup; 
  Real* restrict s_lup = wk-> s_lup;
  Real* restrict h_lup = wk-> h_lup;

  dmatrix_to_zero(wk-> oper);

  k = 0;
  while(K - k) { 

    Real accum = quad[k];
      
    h_lup[0] = exp( - accum * accum / 2 ) / sqrt( sqrt(PI) );
    h_lup[1] = sqrt(2) * accum * h_lup[0];

    i = 2;
    while(M - i) {

      h_lup[i] = sqrt(2.0/i) * accum * h_lup[i-1] - sqrt((i-1)*1.0/i)*h_lup[i-2];   

      i++;
    }

    Real c = c_lup[k];
    Real s = s_lup[k];

    Real c_i  = 1.0;
    Real s_i  = 0.0;

    Real trace = 0.0;

    i = 0;
    while(M - i) {

      Real c_ij = c_i;
      Real s_ij = s_i;

      Real* restrict pr_re = proj_re[i];
      Real* restrict pr_im = proj_im[i];
      Real* restrict st_re = state_re[i];
      Real* restrict st_im = state_im[i];

      j = 0;
      while(M - j) {

        accum = h_lup[i]*h_lup[j];

        Real a = c_ij * accum;
        Real b = s_ij * accum;

        pr_re[j] = a; 
        pr_im[j] = b; 
 
        trace += a * st_re[j] - b * st_im[j];  

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

    trace = 1.0 / trace;

    i = 0;
    while(M - i) {
      j = 0;
      while(M - j) {
        oper_re[i][j] += trace * proj_re[i][j]; 
        oper_im[i][j] += trace * proj_im[i][j]; 
        j++;
      }
      i++;
    }
    k++;
  }

  //printf("finishing pmaxlike_getproj, thread #%i\n", (int) pthread_self());
  pthread_exit(NULL);

}


EXT Real* pmaxlike_iteration
  (maxlike_wk* max, int J)
{

  int i, j, k, theta, x;
  int M = max-> state-> n + 1;
  int T = max-> data-> gd-> x-> n_val;
  int X = max-> data-> gd-> y-> n_val;

  Real trace, reg;

  dmatrix* operator = dmatrix_alloc(max->state->n);
  dmatrix* prod     = dmatrix_alloc(max->state->n);
  dmatrix* last     = dmatrix_alloc(max->state->n);
  dmatrix* state    = max->state;

  Real* err = (Real*) malloc( J * sizeof(Real) );

  int iter = max-> iteration;
  max-> iteration += J;
  J = max-> iteration;

  k = 0;
  while(J-iter) {

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

    theta = 0;
    while(T-theta) { 

      x = 0;
      while(X-x) { 

        reg = max-> data-> val[theta][x];

        if ( reg != 0.0 ) {

          trace = 0.0;

          i = 0;
          while(M-i) {
            j = 0;
            while(M-j) {
              trace += max-> proj_re[theta][x][i][j] * state-> re[i][j]
                    -  max-> proj_im[theta][x][i][j] * state-> im[i][j];
              j++;
            }
            i++;
          }

          trace = reg / trace;

          i = 0;
          while(M-i) {
            j = 0;
            while(M-j) {
              operator-> re[i][j] += trace * max-> proj_re[theta][x][i][j]; 
              operator-> im[i][j] += trace * max-> proj_im[theta][x][i][j]; 
              j++;
            }
            i++;
          }
       
        }

        x++;
      }

      theta++;
    }

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
    if( iter % 50 == 0) 
      printf("%i iterations done\n",iter);
    
  }

  dmatrix_free(operator);
  dmatrix_free(prod);
  dmatrix_free(last);

  return err;

}


/*
EXT wigner* pmaxlike_margi_dispersion
  (marginal* mg, grid* gd, int n, int N, int iter)
{
  
  fock_workspace* f = fock_alloc(gd, N);
  fock_calculate(f);
  maxlike_margi_wk* max = maxlike_margi_alloc(N, mg); 

  wigner* reconstruct(marginal* m, grid* g) { 

    dmatrix_to_Id(max-> state);
    max-> data = m;
    max-> iteration = 0;

    pmaxlike_margi_iteration(max, iter);

    return fock_wig_from_matrix(f, max-> state);

  }

  wigner* dispersion = wigner_dispersion(mg, gd, n, reconstruct);

  fock_free(f);
  maxlike_margi_free(max);

  return dispersion;

}
*/


