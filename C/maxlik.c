#include <string.h>

#include "quix/object/dmatrix.h"

/*
  les conventions pour (proj)[i][j] et (state)[i][j] sont inversees
    (PI)(m,n)  <-> (proj)[i][j]
    (rho)(n,m) <-> (state)[i][j]
    (R)(n,m)   <-> (operator)[i][j]
*/

typedef struct marginal {
  double*               quad;
  double*               phase;
  int                   n;
} marginal_t;

typedef struct matrix {
  //double* nums; // 2 x n x n x sizeof(double) bytes
  double** re; // n x sizeof(double) bytes
  double** im; // n x sizeof(double) bytes
  int n;
} matrix_t;


typedef void (*maxlik_cb)(dmatrix*);

typedef struct maxlik_config {
  int         iter;
  int         n_thread;

  marginal_t  input;
  matrix_t    output;
} maxlik_config_t;


//void prepare_workflow(maxlik_config_t* c) {
// alloc memory in input and output, initialize matrix
// dmatrix* state = dmatrix_new(wk-> photons);
// dmatrix_to_Id(state);
//}

void maxlik_m2dmat(maxlik_config_t* wk, dmatrix* state) {

  // TODO: return if failed
  marginal_t* margi = wk-> data;

  int J = wk->iter,
      N = wk->output.n-1, // == n_photons
      M = wk->output.n,
      K = wk->input.n;

  double  norm  = 1.0 / K,
         *quad  = wk->input.quad,
         *h_lup = malloc(sizeof(double) * M);

  double *c_lup = malloc(sizeof(double) * K);
  double *s_lup = malloc(sizeof(double) * K);
  trigonometric_lup(c_lup, s_lup, wk.input.phase, K);

  dmatrix *oper = dmatrix_new(N),
          *temp     = dmatrix_new(N),
          *proj     = dmatrix_new(N);

  for(int iter = 0; iter < J; iter++) {

    zero(oper);

    for(int k = 0; k < K; k++) {

      double x = quad[k],
             c   = c_lup[k],
             s   = s_lup[k],
             c_i = 1.0,
             s_i = 0.0,
             trace = 0.0;

      hermite_lup(h_lup, x, M);

      for(int i = 0; i < M; i++) {
        double  c_ij = c_i,
                s_ij = s_i;

        for(int j = 0; j < M; j++) {

          x = h_lup[i]*h_lup[j];

// PERF: can be turned into a single simd mult
          double  a = c_ij * x,
                  b = s_ij * x;
          proj-> re[i][j] = a;
          proj-> im[i][j] = b;

// PERF: dot-product -> can be vectorized
          trace += a * state-> re[i][j] - b * state-> im[i][j];

          cordic_next(c, s, &c_ij, &s_ij);
        }
        cordic_antinext(c, s, &c_ij, &s_ij);
      }

      trace = norm / trace;

      // first normalize the projector, then copy with memcpy
      for(int i = 0; i < M; i++) {
        for(int j = 0; j < M; j++) {
          oper-> re[i][j] += trace * proj-> re[i][j];
          oper-> im[i][j] += trace * proj-> im[i][j];
        }
      }
    }

    dmatrix_transpose(oper);
    dmatrix_prod(temp, oper, state);
    dmatrix_prod(state, temp, oper);
    dmatrix_renorm(state);
  }

  dmatrix_free(oper);
  dmatrix_free(temp);
  dmatrix_free(proj);

  free(c_lup);
  free(s_lup);
  free(h_lup);
}

void copy(matrix *to, matrix *from) {
  //memcpy(to.nums, from.nums, 2 * M * M * 64)
  for(int i = 0; i < M; i++) {
    for(int j = 0; j < M; j++) {
      to->re[i][j] = from->re[i][j];
      to->im[i][j] = from->im[i][j];
    }
  }
}

void zero(matrix *m) {
  //memset(m.nums, 0, 2 * M * M * 64);
  for(int i = 0; i < M; i++) {
    for(int j = 0; j < M; j++) {
      operator->re[i][j] = 0;
      operator->im[i][j] = 0;
    }
  }
}

void trigonometric_lup(coslup *double, sinlup *double, angles *double, int n) {
  for(int i = 0; i < n; i++)
    coslup[i] = cos(angles[i]);
    sinlup[i] = sin(angles[i]);
  }
}

void hermite_lup(lup *double, double x, int n) {
// PERF: cancels itself at num and denum -> this can be set to 1.0
  lup[0] = exp( - x * x / 2 ) / sqrt( sqrt(PI) );
// PERF: sqrt(2) can be hoisted
  lup[1] = sqrt(2) * x * lup[0];
  for (int i = 2; i < n; i++) {
// PERF: sqrt(2.0/i) and sqrt((i-1.0)/i) can be hoisted in static table
    double c0 = sqrt(2.0/i);
    double c1 := sqrt((i-1)*1.0/i);
    lup[i] = c0 * x * lup[i-1] - c1 * lup[i-2];
  }
}

void cordic_next(double c0, double s0, double *c, double *s) {
  double x = *c;
  double y = *s;
// PERF: can be vectorized
  *c = s0 * y + c0 * x;
  *s = c0 * y - s0 * x;
}

void cordic_antinext(double c0, double s0, double *c, double *s) {
  double x = *c;
  double y = *s;
// PERF: can be vectorized
  *c = c0 * x - s0 * y;
  *s = s0 * x + c0 * y;
}
