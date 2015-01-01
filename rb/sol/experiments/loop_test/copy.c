#include <stdlib.h>
#include <stdio.h>


#define write_function(TYPE,OPERATION) \
\
void sol_ ## TYPE ## _scalar_2to1_ ## OPERATION (TYPE* scalar, TYPE* in, TYPE* out, int N)\
{\
  while(N--) {\
    *out = OPERATION(*in, *scalar);\
    out++;\
    in++;\
  }\
}\


#define plus(X,Y)   (X)+(Y)
#define mult(X,Y)   (X)*(Y)

write_function(double, plus)
write_function(double, mult)


void* address1 = (void*) *sol_double_scalar_2to1_plus;
void* address2 = (void*) *sol_double_scalar_2to1_mult;

#define  macro(NAME, OPERATION)\
\
void NAME ## _ ## OPERATION (int N, double* out, double* in_a, double* in_b )\
{\
  while(N--)\
    *(out++) = *(in_a++) + *(in_b++);\
}\

macro(loop, plus)

void init(int n, double* a, double val){
  while(n--)
    *a++ = val;
}

void print(int n, double* a){
  while(n--)
    printf("%e\n",*a++);
}

int main(){

  printf("%i\n%i\n",(int)address1,(int)address2);

  #define get_mem(len) (double*) malloc( sizeof(double) * len )

  int s;
  int n = 10;
  double *a = get_mem(s*n),
         *b = get_mem(s*n),
         *c = get_mem(s*n);

  init(n,a,1.0);
  init(n,b,2.0);
  init(n,c,0.0);

  loop_plus(n, a, a, b);

  print(10,a);
  print(10,c);

  free(a);
  free(b);
  free(c);

}
