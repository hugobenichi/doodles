#include "invmax.h"


EXT wigner* 
invmaxlike
(marginal* m, grid* g, int I)
{


  #define   AMP           65535
  #define   MAX           15.0
  #define   REZ           (MAX/AMP)
  #define   TO_INT(x)     round(x)


  int iter, k, r, t;
  int K = m-> samples;
  int R = g->x->n_val;
  int T = g->y->n_val;

  double  sigma = 0.1;
  double  norm  = 1.0 / K;
  double* r_val = g->x->val;
  double* t_val = g->y->val;


  double* lup = (double*) malloc( sizeof(double)*AMP );
  double rad = 0.0;
  for (k = 0; k < AMP; k++) {
    rad += REZ;
    lup[k] = exp( - rad*rad / (2*sigma*sigma) ) / ( sqrt(2.0*PI)*sigma ); 
  }



  wigner* w_ret  = wigner_alloc(g);

  range* swap; 

  swap = g->x;
  g->x = g->y;
  g->y = swap;

  wigner* w  = wigner_alloc(g);
  wigner* z1 = wigner_alloc(g);
  wigner* z2 = wigner_alloc(g);

  swap = g->x;
  g->x = g->y;
  g->y = swap;

  void init(int i, int j) {
    double rad = r_val[j];  // !! inv R <-> T
    double vacuum = exp( -rad*rad ) / PI;
    z1->val[i][j] = exp( vacuum );
  }

  void copy_to_w(int i, int j) {
    w->val[i][j] = log( z1->val[i][j] );
  }

  void w_trans_forward(int i, int j) {
    w_ret->val[i][j] = w->val[j][i];
  }

  void w_trans_backward(int i, int j) {
    w->val[i][j] = w_ret->val[j][i];
  }

  void z2_to_zero(int i, int j) {
    z2->val[i][j] = 0.0;
  }

  wigner_loop(init, T, R);              // !! inv R <-> T
  //wigner_loop(init, R, T);

  wigner_loop(copy_to_w, T, R);         
  wigner_loop(w_trans_forward, R, T);   

  
  void checknan(int i, int j){
    if ( isnan( z1->val[i][j] ) ) 
      printf("nan in z at (%i,%i)\n",i,j);
  }

  wigner_loop(checknan, T, R);

  for (iter = 0; iter < I; iter++)
  {

    wigner_loop(z2_to_zero, T, R);  // !! inv R <-> T
    //wigner_loop(to_zero, R, T);

    for (k = 0; k < K; k++)
    {

      double q    = m-> quad[k] / REZ;
      double phi  = m-> phase[k];
      double prob = 0.0;

      for (t = 0; t < R; t++)
      {
        double ang = cos(t_val[t] - phi) / REZ;
        for (r = 0; r < R; r++)
        {
          int index = (int) TO_INT( fabs( q - r_val[r] * ang ) );
          prob += r_val[r] * w->val[t][r] * lup[index];  // !! inv r <-> t
          //prob += r_val[r] * w->val[r][t] * lup[index];
        }
      }
  
      prob *= grid_cell(g); 

      for (t = 0; t < R; t++)
      {
        double ang = cos(t_val[t] - phi) / REZ;
        for (r = 0; r < R; r++)
        {
          int index = (int) TO_INT( fabs( q - r_val[r] * ang ) );
          z2->val[t][r] += lup[index] / ( prob * z1->val[t][r]);  // !! inv r <-> t
          //z2->val[r][t] += lup[index] / ( prob * z1->val[r][t]);  // !! inv r <-> t
        }
      }

    }

    for (t = 0; t < R; t++)
      for (r = 0; r < R; r++)
        z1->val[t][r] = K / ( z2->val[t][r] );  // !! inv r <-> t
        //z1->val[r][t] = 1.0 / ( z2->val[r][t] * K );  // !! inv r <-> t

    wigner_loop( copy_to_w, T, R);

    wigner_loop( w_trans_forward, R, T);

    wigner_renorm(w_ret);
    //wigner_renorm(w);

    wigner_loop( w_trans_backward, T, R);

    printf("iteration %i finished\n",iter);

  } 

  wigner_loop(checknan, T, R);

  //wigner_free(z1);
  wigner_free(z2);
  wigner_free(w);
  free(lup);

  return z1; //w_ret;

  #undef    A
  #undef    S
  #undef    R
  #undef    TO_INT

}


