#ifndef _FILTER_
  #define _FILTER_

  #include  <stdlib.h>
  #include	<stdio.h>
  #include  <math.h>
  #include  <memory.h>

  #define     Doub        double
  #define     Sing        float
  #define     Real        Doub
  #define     PI          3.1411592654

  #ifdef __WIN32__
    #define EXT(x) extern __declspec(dllexport) x
  #else
    #define EXT(x) x
  #endif

  #define 	DLL 			extern __declspec(dllexport)

typedef struct iir_proto {
  int length;
  double* cooef;
} iir

typedef struct fir_proto {
  int length;
  double* cooef;
} fir

EXT() void iir_alloc(int length, double* coef);
EXT() void iir_free(iir* f);
EXT() void iir_apply(iir* f, char* data, int dat_siz);

EXT() void fir_alloc(int length, double* coef);
EXT() void fir_free(fir* f);
EXT() void fir_apply(fir* f, char* data, int dat_siz);

#endif
