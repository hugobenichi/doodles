

#include "matr.h"


matr*   
matr_alloc 
(int n)
{

  matr* mat = (matr*) malloc( sizeof(matr) );

  int m = 0;

  while( m * cache_d < n ) m += 1;

  n = m * cache_d;
  mat-> n_phot  = n;
  mat-> n_dbls  = m;

  doubles** re = mat-> re;
  doubles** im = mat-> im;

  re  = (doubles**) malloc( sizeof(doubles*) * n );
  im  = (doubles**) malloc( sizeof(doubles*) * n );

/*
  re[0] = (doubles*) malloc( sizeof(doubles) * n * m ); 
  im[0] = (doubles*) malloc( sizeof(doubles) * n * m ); 

  int i;
  for(i = 1; i < n; i++)
  {
    re[0] = re[0] + i * m;
    im[0] = re[0] + i * m;
  }
*/


  int i;
  for(i = 0; i < n; i++)
  {
    re[i] = (doubles*) malloc( sizeof(doubles) * m );
    im[i] = (doubles*) malloc( sizeof(doubles) * m );
  }


  return mat;

}


void    
matr_free  
(matr* mat)
{

  //free( mat-> re[0] );
  //free( mat-> im[0] );

  free( mat-> re );
  free( mat-> im );

  free( mat);

}


void    
matr_copy  
(matr* mat, dmatrix* d)
{

  int N = mat-> n_phot;
  int M = mat-> n_dbls;

  int i, j, k, kk;

  j = 0;
  for( i = 0; i < N; i++)
  {

    Real*     d_re    = d-> re[i];
    Real*     d_im    = d-> im[i];
    doubles*  mat_re  = mat-> re[i];
    doubles*  mat_im  = mat-> im[i];

    for( k = 0; k < M; k++ )
    {
      for( kk = 0; k < cache_d; kk++)
      {
DEB(i,i * 10000 + k * 100 + kk);
        mat_re[k][kk] = d_re[j];
        mat_im[k][kk] = d_im[j];
        j++;
      }
    }
  }

}


void    
matr_zero  
(matr* mat)
{

  int N = mat-> n_phot;
  int M = mat-> n_dbls;
  int i, k, kk;

  for( i = 0; i < N; i++)
  {
    doubles*  mat_re  = mat-> re[i];
    doubles*  mat_im  = mat-> im[i];
    for( k = 0; k < M; k++ )
    {
      for( kk = 0; k < cache_d; kk++)
      {
DEB(i,i * 10000 + k * 100 + kk);
        mat_re[k][kk] = 0.0;
        mat_im[k][kk] = 0.0;
      }
    }
  }

}


