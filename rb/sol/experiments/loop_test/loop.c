
#ifdef HOT
  #define HOT_TAG  __attribute__ ((hot))
#else
  #define HOT_TAG
#endif


typedef  double (*operator)(double,double);


void loop_scal(int, double*, double*, double*, double* ) HOT_TAG;
void loop_plus(int, double*, double*, double* ) HOT_TAG;
void loop_plus_inplace(int, double*, double* ) HOT_TAG;
void loop_plus_stride(int, double*, double*, double*, int ) HOT_TAG;
void loop_plus_2(int, int, double*, double*, double* ) HOT_TAG;
void loop_scal_mult(double*, double*, double, int ) HOT_TAG;



void 
loop_scal
(int N, double* out, double* in_a, double* in_b, double* in_c )
{
  int i;
  for (i=0; i<N; i++)
    *(out++) = *(in_c++) * ( *(in_a++) + *(in_b++) );
}


void 
loop_plus
(int N, double* out, double* in_a, double* in_b )
{
  while(N--)
    *(out++) = *(in_a++) + *(in_b++);  //
}


void 
loop_plus_inplace
(int N, double* in_a, double* in_b )
{
  while(N--) {
    *in_a = *in_a++ + *in_b++;  //
  }
}


void 
loop_plus_stride
(int N, double* out, double* in_a, double* in_b, int stride )
{
  while(N--) {
   *out   = *in_a + *in_b;  //
    out  += stride;
    in_a += stride;
    in_b += stride;
  }
}


void 
loop_plus_2
(int N, int offset, double* out, double* in_a, double* in_b )
{
  while( N  ) {
    double *a = in_a, *b = in_b, *c = out;
    int i = offset;
    while(i--)
      *(c++) = *(a++) + *(b++);  //

    in_a += offset, in_b += offset, out += offset;
    N -= offset;
  }

}


void 
loop_scal_mult
(double* out, double* in, double factor, int N )
{
  while(N--)
    *(out++) = *(in++) * factor;  //
}



void 
loop_apply
(int N, operator op, double* out, double* in_a, double* in_b )
{
  int i;
  for (i=0; i<N; i++)
    *(out++) = op(*(in_a++),*(in_b++));
}


double op_plus(double x, double y) { return x + y; }
double op_time(double x, double y) { return x * y; }

operator get_plus(void) { return op_plus; }
operator get_time(void) { return op_time; }

#ifdef MAIN

#include <stdlib.h>

void init(int n, double* a, double* b, double* c, double val)
  //int n, double* a, double* b, double* c
{
  while(n--) {
    *a++ = val;
    *b++ = val;
    *c++ = val;
  };
}

int main(){

  #define get_mem(len) (double*) malloc( sizeof(double) * len )

  int s;
  s = 16;
  int n = 4096;
  double *a = get_mem(s*n);
  double *b = get_mem(s*n);
  double *c = get_mem(s*n);

  s = 1;

  double factor = 0.89567;
  int repeat = 100;

  while(repeat--) {
    init(s*n,a,b,c, 1.0);
    int N = 1000;
    while(N--)
      //loop_scal_mult(b, a, &factor, n);
      loop_scal_mult(a, a, factor, n);
      //loop_plus(n, c, a, b);
      //loop_plus(n, a, a, b);
      //loop_plus_inplace(n, a, b);
      //loop_plus_stride(n, c, a, b, s);
      //loop_plus_stride(n, a, a, b, s);
  }

  free(a);
  free(b);
  free(c);

}

#endif

