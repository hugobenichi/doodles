#include <stdio.h>


double 
scal_plus_0
(int N,  double* in_a, double* in_b )
{

  int i;
  double rez = 0.0;
  for (i=0; i<N; i++)
    rez += *(in_a++) + *(in_b++);
  return rez;

}


void
scal_plus_1
(int N, double* out, double* in_a, double* in_b )
{

  int i;
  *out = 0.0;
  for (i=0; i<N; i++)
    *out += *(in_a++) + *(in_b++);

}


void
scal_plus_2
(int N, double* out, double* in_a, double* in_b )
{

  //printf("N init: %i\n",N);

  //int i = 0;

  *out = 0.0;
  while (N--) {
    *out += *(in_a++) + *(in_b++);
  }

  //printf("N final: %i %i\n",N,i);

}


void
scal_plus_3
(int N, double* out, double* in_a, double* in_b, int stride[] )
{

  int a = stride[0], b = stride[1], c = stride[2];
  int i = 0, j = 0, k = 0;
  
  while (N--) {
    *out += *(in_a) + *(in_b);
    in_a += a;
    in_b += b;
    out  += c;  
  }

  //printf("N final: %i %i\n",N,i);

}


void
scal_plus_4
(int N, double* out, double* in_a, double* in_b, int stride[] )
{

  int a = stride[0], b = stride[1], c = stride[2];
  int i = 0, j = 0, k = 0;
  
  while (N--) {
    out[k]  += in_a[i] + in_b[j];
    i += a;
    j += b;
    k += c;  
  }

  //printf("N final: %i %i\n",N,i);

}


void
scal_plus_5
(int N, double* out, double* in_a, double* in_b )
{

  while (N--) {
    *out += *(in_a) + *(in_b);
    in_a ++;
    in_b ++;
    out  ++;   
  }

  //printf("N final: %i %i\n",N,i);

}

