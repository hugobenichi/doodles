#include "maxlike.h"


#define   Check(p,msg)    if (p == NULL) printf("\n!!! NULL malloc: %s \n",msg)


EXT maxlike_wk* maxlike_alloc
  (int N, histogram* h)
{

  maxlike_wk* max = (maxlike_wk*) malloc(sizeof(maxlike_wk));
  Check(max,"max");

  max-> print     = 0;
  max-> iteration = 0;
  max-> data      = h;
  max-> state     = dmatrix_alloc(N);
  max-> tester    = NULL;

  dmatrix_to_Id(max-> state);

  int theta, x, i;
  int M = N + 1;
  int T = h-> gd-> x-> n_val;
  int X = h-> gd-> y-> n_val;

  max-> proj_re = (Real****) malloc( sizeof(Real***) * T ); 
  max-> proj_im = (Real****) malloc( sizeof(Real***) * T );

  Check(max-> proj_re,"proj_r****");
  Check(max-> proj_im,"proj_i****");

  for(theta = 0; theta < T; theta++) {

    max-> proj_re[theta] = (Real***) malloc( sizeof(Real**) * X );
    max-> proj_im[theta] = (Real***) malloc( sizeof(Real**) * X );

    Check(max-> proj_re[theta],"proj_r***");
    Check(max-> proj_im[theta],"proj_i***");

    for(x = 0; x < X; x++) {

      max-> proj_re[theta][x] = (Real**) malloc( sizeof(Real*) * M );
      max-> proj_im[theta][x] = (Real**) malloc( sizeof(Real*) * M );

      Check(max-> proj_re[theta][x],"proj_r**");
      Check(max-> proj_im[theta][x],"proj_i**");

      for(i = 0; i < M; i++) {

        max-> proj_re[theta][x][i] = (Real*) malloc( sizeof(Real) * M );
        max-> proj_im[theta][x][i] = (Real*) malloc( sizeof(Real) * M );

        Check(max-> proj_re[theta][x][i],"proj_r*");
        Check(max-> proj_im[theta][x][i],"proj_i*");

      }

    }

  }

  return max;

}


EXT void maxlike_free
  (maxlike_wk* max)
{

  int theta, x, i;
  int M = max-> state-> n + 1;
  int T = max-> data-> gd-> x-> n_val;
  int X = max-> data-> gd-> y-> n_val;

  for(theta = 0; theta < T; theta++) {

    for(x = 0; x < X ; x++) {

      for(i = 1; i < M; i++) {

        if ( max-> proj_re[theta][x][i] != NULL ) free( max-> proj_re[theta][x][i]);
        if ( max-> proj_im[theta][x][i] != NULL ) free( max-> proj_im[theta][x][i]);

      }

      if ( max-> proj_re[theta][x] != NULL ) free( max-> proj_re[theta][x]);
      if ( max-> proj_im[theta][x] != NULL ) free( max-> proj_im[theta][x]);

    }

    if ( max-> proj_re[theta] != NULL ) free( max-> proj_re[theta]);
    if ( max-> proj_im[theta] != NULL ) free( max-> proj_im[theta]);

  }

  max-> data = NULL;

  free( max-> state );
  free( max );

}


EXT dmatrix* maxlike_state
  (maxlike_wk* max)
{

  return dmatrix_copy(max->state);

}

EXT void maxlike_precalculate
  (maxlike_wk* max)
{


  #define H_Amp           10.0
  #define H_Step          65535
  #define H_Rez           (H_Amp/H_Step)


  int i = 0, j = 0, theta = 0, x = 0, y = 0, q = 0;
  int M = max-> state-> n + 1;
  int T = max-> data-> gd-> x-> n_val;
  int X = max-> data-> gd-> y-> n_val;
  int Q = H_Step;

  int sign = 1;

  Real accum, thres, a, b;
  Real rez = max-> data-> gd-> y-> resolution;

  range* r = range_single(H_Step, H_Amp);
  hermite_workspace* h  = hermite_alloc(max-> state-> n, r);
  hermite_calculate(h);

  Real** c_lup = (Real**) malloc( sizeof(Real*) * M );
  Real** s_lup = (Real**) malloc( sizeof(Real*) * M );

  i = 0;
  while(M-i) {

    c_lup[i] = (Real*) malloc( sizeof(Real) * T );
    s_lup[i] = (Real*) malloc( sizeof(Real) * T );

    theta = 0;
    while(T-theta) {
      c_lup[i][theta] = cos( i * (max-> data-> gd-> x-> val[theta]) );
      s_lup[i][theta] = sin( i * (max-> data-> gd-> x-> val[theta]) );
      theta++;
    }  

    i++;
  }

  i = 0;
  while(M - i) {

    j = 0;
    while(i + 1 - j) {

      accum = 0.0;
      thres = rez/2;
      q = 0;
      x = (max-> data-> gd-> y-> n_val)/2;

      while(X - x && Q - q) { 

        accum += h-> harmonics[i][q] * h-> harmonics[j][q];

        if ( r-> val[q] >= thres) {

          thres += rez;
          accum *= r-> resolution;

          theta = 0;
          while(T-theta) {

            max-> proj_re[theta][x][i][j] = c_lup[i-j][theta] * accum;
            max-> proj_im[theta][x][i][j] = s_lup[i-j][theta] * accum;
            theta++;

          }

          x++;
          accum = 0.0;
        }

        q++;
      }  

      j++;
    }

    i++;
  }

  theta = 0;
  while(T-theta) {

    x = (max-> data-> gd-> y-> n_val)/2;
    y = x;

    while(X - x ) { 

      i = 0;
      while(M - i) {

        j = 0;
        while(i + 1 - j) {

            sign = ( (i+j)%2 )? -1 : 1; 

            max-> proj_re[theta][y][i][j] = sign * max-> proj_re[theta][x][i][j];
            max-> proj_im[theta][y][i][j] = sign * max-> proj_im[theta][x][i][j];

            j++;
          }

        i++;
      }  

      x++;
      y--;
    }

    theta++;
  }

  dmatrix* buf = dmatrix_alloc(M-1);

  theta = 0;
  while(T-theta) {

    x = 0;
    while(X - x ) { 

      i = 0;
      while(M - i) {

        j = 0;
        while(i + 1 - j) {

            buf-> re[i][j] = max-> proj_re[theta][x][i][j];
            buf-> im[i][j] = max-> proj_im[theta][x][i][j];

            j++;
          }

        i++;
      }  

      i = 0;
      while(M - i) {

        j = 0;
        while(i + 1 - j) {

            max-> proj_re[theta][x][j][i] = buf-> re[i][j];
            max-> proj_im[theta][x][j][i] =-buf-> im[i][j];

            j++;
          }

        i++;
      }  

      x++;
    }

    theta++;
  }

  dmatrix_free(buf);

  for (i = 0; i < M; i++) {
    free( c_lup[i] ); 
    free( s_lup[i] ); 
  }

  free(c_lup); 
  free(s_lup);

  hermite_free(h); 
  range_free(r);


  #undef H_Amp
  #undef H_Step
  #undef H_Rez

}


EXT Real* maxlike_iteration
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
    if( iter % 10 == 0) 
      printf("%i iterations done\n",iter);
    
  }

  dmatrix_free(operator);
  dmatrix_free(prod);
  dmatrix_free(last);

  return err;

}

#undef Check


