#include "filter.h"

EXT() void iir_alloc(int length, double* coef);
EXT() void iir_free(iir* f);
EXT() void iir_apply(iir* f, char* data, int dat_siz);

EXT() void fir_alloc(int length, double* coef);
EXT() void fir_free(fir* f);
EXT() void fir_apply(fir* f, char* data, int dat_siz);

