

#include "quix/tomography/series.h"


EXT 
series* 
series_new
(int n_ang, int n_rad, double max)
{

  series* ft = quix_object_alloc(series);
  ft-> siz_ang = n_ang;
  ft-> siz_rad = n_rad;
  ft-> amp_rad = max;

  int i;
  ft -> c_re  = quix_alloc(double*, n_ang);
  ft -> c_im  = quix_alloc(double*, n_ang);
  loop(i,n_ang) {
    (ft -> c_re)[i] = double_alloc( n_rad );
    (ft -> c_im)[i] = double_alloc( n_rad );
  }

  return ft;

}


EXT 
void 
series_free
(series* ft)
{
  
  if ( quix_object_free(ft) )
  {
    int i, n_ang = ft-> siz_ang;
    if (ft-> c_re && ft-> c_im)
    {
      loop(i,n_ang) {
        free( ft -> c_re[i] );
        free( ft -> c_im[i] );
      }
      free( ft->c_re );
      free( ft->c_im );
      ft->c_re = NULL;
      ft->c_im = NULL;
    } else already_null("Series");
    free(ft);
  } else already_free("Series");

}


EXT wigner* series_wigner
  (series* ft, grid* g)
{

  if_cartesian(g) return NULL;

  wigner* w = wigner_new(g);

  int       i, j, radial, theta,
            r_max = g-> x-> n_val,
            t_max = g-> y-> n_val,
            n_ang = ft-> siz_ang,
            n_rad = ft-> siz_rad;

  double    L_max = ft-> amp_rad,
          **c_re  = ft -> c_re,
          **c_im  = ft -> c_im,
          **z_lup = quix_alloc(double*,n_ang); 

  loop(i,n_ang)
    z_lup[i] = double_alloc( (n_ang + 2*n_rad) );


  loop(radial,r_max)
  {

    double r = (g-> x-> val[radial])/L_max;
    series_zernike(z_lup, n_ang, n_rad, r);

    loop(theta,t_max)
    {

      double  t     = g-> y-> val[theta];
      double *to_w  = &w->val[radial][theta];
             *to_w  = 0.0;

      for (i = 1; i < n_ang; i++)
      { 
        double c = cos( i*t ),
               s = sin( i*t );
        loop(j,n_rad)
          *to_w += 2*z_lup[i][i+2*j] * ( c_re[i][j] * c + c_im[i][j] * s );
      }

      loop(j,n_rad)
        *to_w += c_re[0][j] * z_lup[0][2*j]; // L_max;

      *to_w /= L_max;

    }    
  }

  loop(i,n_ang) free(z_lup[i]);
  free(z_lup);

  return w;

}


EXT double      
series_negativity               
  (series* ft)
{

  int       j, sign, n_rad = ft-> siz_rad;
  double**  c_re  = ft -> c_re,
            neg = 0.0;

  for( sign = 1, j = 0; j < n_rad; j++, sign *= -1)
    neg += sign * c_re[0][j];

  return neg / ft-> amp_rad;

}


