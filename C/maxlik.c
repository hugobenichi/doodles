#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <nmmintrin.h>
#include <smmintrin.h>

#define PI 3.141592653589793238462643383279502884197169

struct mat {
  double* elem;
  int     n;
  int     n2;
};

struct marginal {
  double* quad;
  double* phase;
  int     n;
};

struct cfg {
  int iter;
  int n_thread;
};

int maxlik_m2dmat(struct cfg*, struct marginal in, struct mat out);

static void marginal_populate(struct marginal);

static void mat_new(struct mat*, int);
static void mat_free(struct mat);
static void mat_transpose(struct mat);
static void mat_prod(struct mat, struct mat, struct mat);
static void mat_renorm(struct mat);
static void mat_to_zero(struct mat);
static void mat_to_id(struct mat);
static void mat_print(struct mat);

static void trigonometric_lup(double* table, double* angles, int n);
static void sqrt_lup(double* table, int n);
static void hermite_lup_setup();
static void hermite_lup(double* lup, double* sqrt_table, double x, int n);
static void cordic_next(__m128d, __m128d, double*);
static void accum_projector(__m128d norm, double* pr, double* op, int n);


/* ---- memory helpers ------------------------------------------------------ */

static void _good_malloc_or_die(void *p, int ln) {
  if (!p) {
    printf("failed malloc at %s:%d\n", __FILE__, ln);
    exit(-1);
  }
}

static void* _must_malloc(size_t nbytes, int ln) {
   void* p = malloc(nbytes);
   _good_malloc_or_die(p, ln);
   return p;
}

#define good_malloc_or_die(ptr) _good_malloc_or_die(ptr, __LINE__)
#define must_malloc(nbytes) _must_malloc((nbytes), __LINE__)

/* -------------------------------------------------------------------------- */


int main(int argc, char* argv[]) {
  const int n_data = 10000;
  const int n_phot = 16;

  struct cfg config = {
    .iter = 20,
    .n_thread = 1,
  };

  struct marginal in = {
    .n = n_data,
    .quad = malloc(sizeof(double) * n_data),
    .phase = malloc(sizeof(double) * n_data),
  };
  good_malloc_or_die(in.quad);
  good_malloc_or_die(in.phase);

  struct mat out;
  mat_new(&out, n_phot);
  good_malloc_or_die(out.elem);
  mat_to_id(out);

  marginal_populate(in);

  const clock_t start = clock();
  maxlik_m2dmat(&config, in, out);
  const clock_t stop = clock();
  printf("maxlik_m2dmat(): %ld\n", stop - start);

  mat_print(out);

  mat_free(out);
  free(in.quad);
  free(in.phase);
}

/* -------------------------------------------------------------------------- */


/*
 * speedups:
 *  - original speed: 250_000 ns
 *  - flattenning (double**,double**) into double*                    -> -2/25
 *  - vectorizing accumulation of proj into oper                      -> nothing !
 *  - vectorizing cordic and trace dot product (not optimal)          -> -3/25
 *  - optimizations in hermite lup                                    -> -2/25
 *    - remove norm
 *    - remove extra sqrt(i) by saving across loop iteration
 *    - remove extra double reciprocal
 *  - vectorizing operator trace                                      -> -2/25
 *  - more optimizations in hermite lup                               -> -1.2/25
 *    - vectorizing iteration rule
 *    - precomputing a table of sqrt of ints
 *
 *  - current best: 148_000 ns
 *
 *  next steps:
 *  - try a single m128d register for cos and sin in cordic_next
 *  - recycle m128d register with lup data in hermite_lup
 *  - vectorize matrix product
 */


/*
  les conventions pour (proj)[i][j] et (state)[i][j] sont inversees
    (PI)(m,n)  <-> (proj)[i][j]
    (rho)(n,m) <-> (state)[i][j]
    (R)(n,m)   <-> (operator)[i][j]
*/
int maxlik_m2dmat(struct cfg* wk, struct marginal input, struct mat output) {
  const int nmat = output.n;
  const int ndata = input.n;
  const double norm = 1.0 / ndata;

  struct mat oper, temp, proj;
  mat_new(&oper, nmat);
  mat_new(&temp, nmat);
  mat_new(&proj, nmat);
  good_malloc_or_die(oper.elem);
  good_malloc_or_die(temp.elem);
  good_malloc_or_die(proj.elem);

  double *h_lup = must_malloc(sizeof(double) * nmat);
  hermite_lup_setup();

  double *sincos_table = must_malloc(sizeof(double) * ndata * 4);
  trigonometric_lup(sincos_table, input.phase, ndata);

  double *sqrt_table = must_malloc(sizeof(double) * nmat * 2);
  sqrt_lup(sqrt_table, nmat);

  for (int iter = 0; iter < wk->iter; iter++) {
    mat_to_zero(oper);

    for (int k = 0; k < ndata; k++) {
      __m128d trace_acc = _mm_set_pd(0, 0);
      // PERF: try putting base cordic cos and sin into a single register.
      __m128d cos_base  = _mm_load_pd(sincos_table + 4 * k);
      __m128d sin_base  = _mm_load_pd(sincos_table + 4 * k + 2);
      // PERF: try not using this register
      __m128d sin_basem = _mm_xor_pd(sin_base, _mm_set1_pd(-0.f)); // sin_basem = - sin_base

      __declspec(align(16)) double sincos_i[2] = { 0.0, 1.0 };
      __declspec(align(16)) double sincos_ij[2];

      hermite_lup(h_lup, sqrt_table, *(input.quad + k), nmat);

      int o = 0;
      for (int i = 0; i < nmat; i++) {
        const double hx = *(h_lup + i);
        *sincos_ij       = *sincos_i;
        *(sincos_ij + 1) = *(sincos_i + 1);
        for (int j = 0; j < nmat; j++) {
          const double hy = hx * *(h_lup + j);
          __m128d x, y;
          x = _mm_loadr_pd(sincos_ij); // PERF: inverse cos/sin to avoid loadr
          x = _mm_mul_pd(x, _mm_set_pd(hy, hy));
          y = _mm_mul_pd(x, _mm_load_pd(output.elem + o));
          trace_acc = _mm_add_pd(trace_acc, y);
          _mm_store_pd(proj.elem + o, x);
          cordic_next(cos_base, sin_base, sincos_ij);
          o += 2;
        }
        cordic_next(cos_base, sin_basem, sincos_i);
      }

      trace_acc = _mm_hsub_pd(trace_acc, trace_acc);
      __m128d trace = _mm_set_pd(norm, norm);
      trace = _mm_div_pd(trace, trace_acc);
      accum_projector(trace, proj.elem, oper.elem, nmat);
    }

    mat_transpose(oper);
    mat_prod(temp, oper, output);
    mat_prod(output, temp, oper);
    mat_renorm(output);
  }

  mat_free(oper);
  mat_free(temp);
  mat_free(proj);

  free(sincos_table);
  free(h_lup);

  return 0;
}


/* ---- numerical helpers --------------------------------------------------- */

static void trigonometric_lup(double* table, double* angles, int n) {
  for (int i = 0; i < n; i++) {
    const double a = *(angles + i);
    const double c = cos(a);
    const double s = sin(a);
    *(table + 4 * i + 0) = c;
    *(table + 4 * i + 1) = c;
    *(table + 4 * i + 2) = s;
    *(table + 4 * i + 3) = s;
  }
}

static void sqrt_lup(double* table, int n) {
  for (int i = 0; i < n; i++) {
    __m128d a = _mm_set_pd(i, i);
    a = _mm_sqrt_pd(a);
    _mm_store_pd(table + 2 * i, a);
  }
}

static double sqrt2;
static double sqrtsqrt_pi;
static void hermite_lup_setup() {
  sqrt2 = sqrt(2.0);
  sqrtsqrt_pi = sqrt(sqrt(PI));
}

static void hermite_lup(double* lup, double* sqrt_table, double x, int n) {
  const double x_sqrt2 = x * sqrt2;
  //*lup = exp( - x * x / 2 ) / sqrtsqrt_pi;  // norm appears both on numerator and denumerator
  *lup       = 1.0;
  *(lup + 1) = x_sqrt2;
  __m128d one = _mm_set_pd(1.0, 1.0);
  __m128d b = _mm_set_pd(1.0, x_sqrt2);
  for (int i = 2; i < n; i++) {
    __m128d a = _mm_load_pd(sqrt_table + 2 * i);
    __m128d n = _mm_shuffle_pd(b, a, 2); // left of a and right of b
    a = _mm_div_pd(one, a);
    a = _mm_mul_pd(a, b);
    b = _mm_set_pd(lup[i-2], lup[i-1]); // PERF: shuffle from previous iteration ?
    a = _mm_mul_pd(a, b);
    a = _mm_hsub_pd(a, a);
    _mm_storeh_pd(lup + i, a);
    b = n;
  }
}

static void cordic_next(__m128d c0, __m128d s0, double* x) {
  __m128d a, b;
  a = _mm_load_pd(x);
  b = _mm_shuffle_pd(a, a, 1);
  a = _mm_mul_pd(a, c0);
  b = _mm_mul_pd(b, s0);
  a = _mm_addsub_pd(a, b);
  _mm_store_pd(x, a);
}

static void accum_projector(__m128d norm, double* pr, double* op, int n) {
  for (int i = 0; i < 2 * n * n; i += 2) {
    __m128d r = _mm_load_pd(op + i);
    __m128d x = _mm_load_pd(pr + i);
    x = _mm_mul_pd(norm, x);
    r = _mm_add_pd(x, r);
    _mm_store_pd(op + i, r);
  }
}


/* ---- matrix functions ---------------------------------------------------- */

#define offset(m,i,j)  ((i) * (m) + (j))

static void mat_new(struct mat* m, int n) {
  m->n    = n;
  m->n2   = n * n;
  m->elem = malloc(2 * m->n2 * sizeof(double*));
}

static void mat_free(struct mat m) {
  free(m.elem);
}

static void mat_to_zero(struct mat m) {
  memset(m.elem, 0, 2 * m.n2 * sizeof(double));
}

static void mat_to_id(struct mat m) {
  mat_to_zero(m);
  const double t = 1.0 / (m.n);
  const int off = 2 + 2 * m.n;
  const int end = 2 * m.n2;
  for (int i = 0; i < end; i += off) {
    *(m.elem + i) = t;
  }
}

static void mat_transpose(struct mat m) {
  for (int i = 0; i < m.n; i++) {
    for (int j = i; j < m.n; j++) {
      const int o = 2 * offset(m.n, i,j);
      const int p = 2 * offset(m.n, j,i);
      double t      = *(m.elem + o);
      *(m.elem + o) = *(m.elem + p);
      *(m.elem + p) = t;
      t                 =   *(m.elem + o + 1);
      *(m.elem + o + 1) = - *(m.elem + p + 1);
      *(m.elem + p + 1) = -t;
    }
  }
}

static double mat_trace(struct mat m) {
  double t = 0.0;
  const int off = 2 + 2 * m.n;
  const int end = 2 * m.n2;
  for (int i = 0; i < end; i += off) {
    t += m.elem[i];
  }
  return t;
}

static void mat_renorm(struct mat m) {
  const double t = 1.0/mat_trace(m);
  __m128d ts = _mm_set_pd(t,t);
  for (int i = 0; i < 2 * m.n2; i += 2) {
    __m128d x = _mm_load_pd(m.elem + i);
    x = _mm_mul_pd(ts, x);
    _mm_store_pd(m.elem + i, x);
  }
}

void mat_prod(struct mat e, struct mat d, struct mat c) {
  for (int i = 0; i < d.n2; i += d.n) {
    for (int j = 0; j < d.n; j++) {
      const int o = 2 * (i + j);
      e.elem[o] = 0.0;
      e.elem[o+1] = 0.0;
      for (int k = 0; k < d.n; k++) {
        const int p = 2 * (i + k);
        const int q = 2 * offset(d.n, k, j);
        e.elem[o] += d.elem[p] * c.elem[q] - d.elem[p+1] * c.elem[q+1];
        e.elem[o+1] += d.elem[p] * c.elem[q+1] + d.elem[p+1] * c.elem[q];
      }
    }
  }
}

static void mat_print(struct mat m) {
  for (int i = 0; i < m.n2; i += m.n) {
    for (int j = 0; j < m.n; j++) {
      const int o = 2 * (i + j);
      printf("%+.2f%+.2fi  ", m.elem[o], m.elem[o+1]);
    }
    puts("");
  }
}

static void marginal_populate(struct marginal data) {
  for (int i = 0; i < data.n; i++) {
    const double a = ((double) rand()) / ((double) RAND_MAX) * PI;
    const double q = ((double) rand()) / ((double) RAND_MAX) * 2 - 1;
    *(data.phase + i) = a;
    *(data.quad + i)  = q + 3 * cos(a);
  }
}
