

#include "quix/object/dmatrix.h"


#define   dmatrix_loop(index_i,index_j, m)        loop(index_i,m) loop(index_j,m) 
#define   dmatrix_diag_loop(index_i, m)           loop(index_i,m) 


EXT dmatrix* dmatrix_new
  (int n)
{

  dmatrix* d = quix_object_alloc(dmatrix);

  d-> n = n;
  d-> re = quix_alloc(double*,n+1);
  d-> im = quix_alloc(double*,n+1);
  alloc_check(d-> re,"dmatrix_new");
  alloc_check(d-> im,"dmatrix_new");

  int i;
  loop(i,(n+1)) {
    d-> re[i] = double_alloc(n+1);
    d-> im[i] = double_alloc(n+1);
  }

  return d;

}


EXT void dmatrix_free
  (dmatrix* d)
{

  if ( quix_object_free(d) )
  {
    int i, m = d->n +1;
    if( d-> re && d-> im) 
    {
      loop(i,m) {
        free( d-> re[i]);
        free( d-> im[i]);
      }
      free( d-> re);
      free( d-> im);
      d-> re = NULL;
      d-> im = NULL;
    } else already_null("DMatrix::Val");
    free(d);
  } else already_free("DMatrix");

}


EXT dmatrix* dmatrix_qubit
  (int N)
{

  dmatrix* d = dmatrix_new(N);
  dmatrix_to_zero(d);

  d-> re[0][0] = 0.5;
  d-> re[1][1] = 0.5;
  d-> re[0][1] =-0.5;
  d-> re[1][0] =-0.5;

  return d;

}


EXT dmatrix* dmatrix_copy
  (dmatrix* d)
{

  int i, j, m = d-> n +1;

  dmatrix* e = dmatrix_new(d-> n);

  dmatrix_loop(i,j,m) {
    e->re[i][j] = d->re[i][j]; 
    e->im[i][j] = d->im[i][j];
  }

  return e;

}


EXT void dmatrix_copyto
  (dmatrix* d, dmatrix* e)
{

  int i, j, m = d-> n +1;

  dmatrix_loop(i,j,m) {
    e->re[i][j] = d->re[i][j]; 
    e->im[i][j] = d->im[i][j];
  }

}


EXT void dmatrix_to_zero
  (dmatrix* d)
{

  int i, j, m = d-> n +1;

  dmatrix_loop(i,j,m) {
    d->re[i][j] = 0.0; 
    d->im[i][j] = 0.0;
  }

}


EXT void dmatrix_to_Id
  (dmatrix* d)
{

  dmatrix_to_zero(d);

  int i, m = d-> n +1;

  dmatrix_diag_loop(i,m)
    d->re[i][i] = 1.0/m; 

}


EXT void dmatrix_transpose
  (dmatrix* d)
{

  dmatrix* e = dmatrix_copy(d);

  int i, j, m = d-> n +1;

  dmatrix_loop(i,j,m) {
    d->re[i][j] = e->re[j][i]; 
    d->im[i][j] = e->im[j][i];
  }

  dmatrix_free(e);

}


EXT void dmatrix_conjugate
  (dmatrix* d)
{

  dmatrix* e = dmatrix_copy(d);

  int i, j, m = d-> n +1;

  dmatrix_loop(i,j,m) {
    d->re[i][j] = +e->re[j][i]; 
    d->im[i][j] = -e->im[j][i];
  }

  dmatrix_free(e);

}


EXT void dmatrix_sym
  (dmatrix* d)
{

  int i,j, m = d-> n +1;

  loop(i,m)
    loop(j,i) {
      (d->re)[j][i] = +(d->re)[i][j];
      (d->im)[j][i] = -(d->im)[i][j];
    }

}


EXT double* dmatrix_diagonal
  (dmatrix* d)
{

  int i, m = d-> n +1;

  double* diag = double_alloc(m);
  dmatrix_diag_loop(i,m)
    diag[i] = d->re[i][i];

  return diag;

}


EXT double dmatrix_trace
  (dmatrix* d)
{

  int i, m = d-> n +1;
  double t = 0.0;

  dmatrix_diag_loop(i,m)
    t += d->re[i][i];

  return t;

}


EXT void dmatrix_renorm
  (dmatrix* d)

{

  int i,j, m = d-> n +1;
  double t = 1.0/dmatrix_trace(d);

  dmatrix_loop(i,j,m) {
    d->re[i][j] *= t; 
    d->im[i][j] *= t;
  }

}


EXT double dmatrix_avg_phot
  (dmatrix* d)
{

  int i, m = d-> n +1;
  double n = 0.0;

  dmatrix_diag_loop(i,m)
    n += i * d->re[i][i];

  return n;

}


EXT double dmatrix_purity
  (dmatrix* d)
{

  int i,j, m = d-> n +1;
  double p = 0.0;

  dmatrix_loop(i,j,m)
    p += d->re[i][j] * d->re[i][j] + d->im[i][j] * d->im[i][j];

  return p;

}


EXT double dmatrix_neg
  (dmatrix* d)
{

  int i, m = d-> n +1;
  double neg = 0.0, f = 1.0;

  dmatrix_diag_loop(i,m) {
    neg += f * d->re[i][i]; 
    f *= -1.0;
  }

  return neg / PI;

}


EXT dmatrix* dmatrix_prod_alloc
  (dmatrix* d, dmatrix* c)
{ 

  int i,j,k, m = d-> n +1;
  dmatrix* e = dmatrix_new(d-> n);

  dmatrix_loop(i,j,m) {
    e->re[i][j] = 0.0;   
    e->im[i][j] = 0.0;   
    loop(k,m) {
      e->re[i][j] += d->re[i][k] * c->re[k][j] - d->im[i][k] * c->im[k][j];  
      e->im[i][j] += d->re[i][k] * c->im[k][j] + d->im[i][k] * c->re[k][j];  
    }
  }

  return e;

}


EXT void dmatrix_prod
  (dmatrix* e, dmatrix* d, dmatrix* c)
{ 

  int i,j,k, m = d-> n +1;

  dmatrix_loop(i,j,m) {
    e->re[i][j] = 0.0;   
    e->im[i][j] = 0.0;   
    loop(k,m) {
      e->re[i][j] += d->re[i][k] * c->re[k][j] - d->im[i][k] * c->im[k][j];  
      e->im[i][j] += d->re[i][k] * c->im[k][j] + d->im[i][k] * c->re[k][j];  
    }
  }

}


EXT double dmatrix_dist_max  
  (dmatrix* a,dmatrix* b)
{

  double n = -1.0;
  int i = 0, j = 0, M = a-> n +1;

  while(M - i) {
    double* a_re = a-> re[i];
    double* a_im = a-> im[i];
    double* b_re = b-> re[i];
    double* b_im = b-> im[i];
    while(M - j) {
      double x = a_re[j] - b_re[j];
      double y = a_im[j] - b_im[j];
      double m = x * x + y * y;

      if(likely(m > n)) n = m;

      j++;
    }
    i++; 
  }
 
  return sqrt(n);

}


EXT double dmatrix_dist_frob 
  (dmatrix* a, dmatrix* b)
{

  double norm = 0.0;
  int i = 0, j = 0, M = a-> n +1;

  while(M - i) {
    double* a_re = a-> re[i];
    double* a_im = a-> im[i];
    double* b_re = b-> re[i];
    double* b_im = b-> im[i];
    while(M - j) {
      double x = a_re[j] - b_re[j];
      double y = a_im[j] - b_im[j];
      norm += x * x + y * y;
      j++;
    }
    i++; 
  }
 
  return sqrt(norm);

}


#undef dmatrix_loop
#undef dmatrix_diag_loop


