#ifndef _QUIX_MATR_
#define _QUIX_MATR_


#include "settings.h"
#include "density_matrix.h"


typedef struct proto_matr
{

  int n_phot;
  int n_dbls;
  doubles** re;
  doubles** im;

} matr;


matr*   matr_alloc (int);
void    matr_free  (matr*);
void    matr_copy  (matr*, dmatrix*);
void    matr_zero  (matr*);


#endif


