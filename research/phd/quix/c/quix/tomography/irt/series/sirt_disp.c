

#include "quix/tomography/irt/irt_mc_variance.h"
#include "quix/tomography/irt/irt_series.h"


/*
 *    gives an error estimate of negativity by iteration of radon_ft coefficients 
 *    calculation (using bootstrap sampling)
 */
EXT   
double*   
sirt_mc_m2neg_var
(int N, series* ft, marginal* (*get)(void) )
{

  int     n;

  double  neg, nrm, 
          avrg  = 0.0, 
          disp  = 0.0,
         *disps = double_alloc( N );

  loop(n,N) {
  
    marginal* m = get();
    neg = sirt_calculate_neg(ft,m),
            
    disp += neg * neg;
    avrg += neg;

    nrm = 1.0 / (n+1);    
    disps[n] = sqrt(  nrm * ( disp - nrm * avrg*avrg ) );

    marginal_free(m);
  
  }

  return disps;

}


EXT   
negvar_wk*   
sirt_mc_m2negvar_wk
(int N, series* ft, marginal* (*get)(void) )
{

  negvar_wk* ng  = irt_negvar_new(N);

  int     n;

  double  neg, nrm,
          avrg  = 0.0, 
          disp  = 0.0;

  loop(n,N) {
  
    marginal* m = get();
    neg = sirt_calculate_neg(ft,m),
           

    disp += neg * neg;
    avrg += neg;
    
    nrm = 1.0 / (n+1);
    ng-> dispersion[n] = sqrt(  nrm * ( disp - nrm * avrg*avrg ) );
    ng-> negativity[n] = neg;
    ng-> variance[n]   = sirt_calc_neg_var(ft,m);

    marginal_free(m);
  
  }

  return ng;

}


/*
EXT 
irts_disp* 
fri_disp_alloc
(int n_ang, int n_rad, double max)
{

  irts_disp* ft = (irts_disp*) malloc( sizeof(irts_disp) );
  ft -> siz_ang = n_ang;
  ft -> siz_rad = n_rad;
  ft -> amp_rad = max;

  int i,j,k;
  ft -> c_re  = (double****) malloc( sizeof(double***)*n_ang );
  ft -> c_im  = (double****) malloc( sizeof(double***)*n_ang );
  for (i = 0; i < n_ang; i++) 
  {
    (ft -> c_re)[i] = (double***) malloc( sizeof(double**)*n_rad );
    (ft -> c_im)[i] = (double***) malloc( sizeof(double**)*n_rad );
    for (j = 0; j < n_rad; j++) 
    {
      (ft -> c_re)[i][j] = (double**) malloc( sizeof(double*)*n_ang );
      (ft -> c_im)[i][j] = (double**) malloc( sizeof(double*)*n_ang );
      for (k = 0; k < n_ang; k++) 
      {
        (ft -> c_re)[i][j][k] = (double*) malloc( sizeof(double)*n_rad );
        (ft -> c_im)[i][j][k] = (double*) malloc( sizeof(double)*n_rad );
      }
    }
  }

  return ft;

}


EXT 
void 
fri_disp_free
(irts_disp* ft)
{
  
  int i,j,k;
  for (i = 0; i < ft -> siz_ang; i++) 
  {
    for (j = 0; j < ft -> siz_rad; j++) 
    {
      for (k = 0; k < ft -> siz_ang; k++) 
      {
        free( ft -> c_re[i][j][k]);
        free( ft -> c_im[i][j][k] );
      }
      free( ft -> c_re[i][j] );
      free( ft -> c_im[i][j] );
    }
    free( ft -> c_re[i] );
    free( ft -> c_im[i] );
  }
 
  free( ft->c_re );
  free( ft->c_im );
  free( ft );

}



/*
 *    calculate radon_ft coefficients and also create an estimate of variances and covariances
 *    for every coefficients (can be drawn as a Wigner function with fri_construct_disp)
 /
EXT   
irts_disp*   
fri_calculate_disp       
(radon_ft* ft, marginal* m)
{


  int       i, j, n;
  int       N     = m -> samples;
  int       n_ang = ft-> siz_ang;
  int       n_rad = ft-> siz_rad;
  double    L_max = ft-> amp_rad;
  double**  c_re  = ft -> c_re;
  double**  c_im  = ft -> c_im;

  radon_ft* var   = fri_alloc(n_ang, n_rad, L_max);
  double**  v_re  = var -> c_re;
  double**  v_im  = var -> c_im;

  radon_ft* cor   = fri_alloc(n_ang, n_rad, L_max);
  double**  r_re  = cor -> c_re;
  double**  r_im  = cor -> c_im;

  double* che_lup = (double*) malloc( (n_ang + 2*n_rad)* sizeof(double) );


  for (i = 0; i < n_ang; i++) 
  {
    for (j = 0; j < n_rad; j++) 
    {
      c_re[i][j] = 0.0;
      c_im[i][j] = 0.0; 
      v_re[i][j] = 0.0;
      v_im[i][j] = 0.0;   
      r_re[i][j] = 0.0;
      r_im[i][j] = 0.0;          
    }
  }

  for (n = 0; n < N; n++) 
  {
    double t =  m -> phase[n];
    double x = (m -> quad[n]) / L_max; 

    if ( fabs(x) > 1.0 ) continue; 

    che_lup[0] = 1.0;
    che_lup[1] = 2.0 * x;  
    for (i = 2; i < n_ang + 2*n_rad; i++)
      che_lup[i] = 2.0 * x * che_lup[i-1] - che_lup[i-2];

    for (i = 0; i < n_ang; i++) 
    {
      double c = cos( i*t ),
             s = sin( i*t );
      for (j = 0; j < n_rad; j++) 
      {
        double re = c * che_lup[i+2*j],
               im = s * che_lup[i+2*j];
        c_re[i][j] += re;
        c_im[i][j] += im;  
        r_re[i][j] += re * che_lup[1];
        r_im[i][j] += im * che_lup[1];  
      }
    }
  }

  for (n = 0; n < N; n++) 
  {
    double t =  m -> phase[n];
    double x = (m -> quad[n]) / L_max; 

    if ( fabs(x) > 1.0 ) continue; 

    che_lup[0] = 1.0;
    che_lup[1] = 2.0 * x;  
    for (i = 2; i < n_ang + 2*n_rad; i++)
      che_lup[i] = 2.0 * x * che_lup[i-1] - che_lup[i-2];

    for (i = 0; i < n_ang; i++) 
    {
      double c = cos( i*t ),
             s = sin( i*t );
      for (j = 0; j < n_rad; j++) 
      {
        double re = N * c * che_lup[i+2*j] - c_re[i][j],
               im = N * s * che_lup[i+2*j] - c_im[i][j];
        v_re[i][j] += re * re;
        v_im[i][j] += im * im;  
      }
    }
  }

  double n1 = 1.0/( N * PI * L_max );
  for (i = 0; i < n_ang; i++) 
  {
    for (j = 0; j < n_rad; j++) 
    {
      double norm = n1 * (i + 2*j + 1);
      c_re[i][j] *= norm;
      c_im[i][j] *= norm;
      v_re[i][j] = sqrt( v_re[i][j] / (N-1) ) * norm;
      v_im[i][j] = sqrt( v_im[i][j] / (N-1) ) * norm;


      r_re[i][j] = 2*v_re[i][j] * norm /(PI*L_max) - c_re[i][j] * c_re[0][1];
      r_im[i][j] = 2*v_re[i][j] * norm /(PI*L_max) - c_re[i][j] * c_re[0][1];

      printf("correl (0,1)(%i,%i):  %f  |  %f\n",i,j,r_re[i][j],r_im[i][j]);

    }
  }

/*
  double n1 = 1.0/( N * PI    * L_max ),
         n2 = 1.0/( N * PI*PI * L_max*L_max );

  for (i = 0; i < n_ang; i++) 
  {
    for (j = 0; j < n_rad; j++) 
    {
      int k = i + 2*j + 1;

      c_re[i][j] *= n1 * k;
      c_im[i][j] *= n1 * k;

      double a = n2*k*k * v_re[i][j] - c_re[i][j]*c_re[i][j] ,
             b = n2*k*k * v_im[i][j] - c_im[i][j]*c_im[i][j] ;

      if ( a < 0.0 || b < 0.0 ) printf("sqrt of 0 !!(%i,%i) -> (%f,%f) \n",i,j,a,b);
 
      v_re[i][j] = sqrt( ( n2*k*k * v_re[i][j] - c_re[i][j]*c_re[i][j] ) / (N-1) );
      v_im[i][j] = sqrt( ( n2*k*k * v_im[i][j] - c_im[i][j]*c_im[i][j] ) / (N-1) );

    }
  }
/
  free(che_lup);

  fri_free(cor);

  return NULL;

}


EXT 
wigner* 
fri_construct_disp
(irts_disp* ft, grid* g)
{

  //printf("constructing wigner function with Zernike polynomials\n");

  wigner* w = wigner_alloc(g);

  int         i, j, k, l, radial, theta;
  int         r_max = g-> x-> n_val;
  int         t_max = g-> y-> n_val;
  int         n_ang = ft-> siz_ang;
  int         n_rad = ft-> siz_rad;

  double      L_max = ft-> amp_rad;
  double      norm  = 1.0/( 2*PI*PI );
  double****  c_re  = ft -> c_re;
  double****  c_im  = ft -> c_im;

  //indices (up,down)
  double**  zer_lup = (double**)  malloc( n_ang * sizeof(double) ); 
  for (i = 0; i < n_ang; i++)
    zer_lup[i] = (double*) malloc( (n_ang + 2*n_rad) * sizeof(double));

  for (radial = 0; radial < r_max ; radial++) 
  {

    double r = (g-> x-> val[radial])/L_max;
    fri_zernike(zer_lup, n_ang, n_rad, r);

    for (theta = 0; theta < t_max; theta++) 
    {

      double  t     = g-> y-> val[theta];
      double  to_w  = 0.0;

      for (i = 1; i < n_ang; i++)
      { 
        double c1 = cos( i*t );
        double s1 = sin( i*t );
        for (j = 0; j < n_rad; j++)
        {

          double z1 = zer_lup[i][i+2*j];
          to_w += z1*z1 * ( c_re[i][j][i][j] * c1*c1 + c_im[i][j][i][j] * s1*s1 );

          for (k = i+1; k < n_ang; k++)
          { 
            double c2 = cos( k*t );
            double s2 = sin( k*t );
            for (l = j+1; l < n_rad; l++)
              to_w += 4*z1*zer_lup[k][k+2*l] * ( 
                      + c_re[i][j][k][l] * c1*c2 
                      + c_re[i][j][k][l] * c1*s2    // !!! attention !!
                      + c_re[i][j][k][l] * s1*c2    // wrong covariance coefficients
                      + c_im[i][j][k][l] * s1*s2 
                    ); 
          }

          for (l = j+1; l < n_rad; l++)      // !!! attention wrong covariance coefficients
            to_w += 2 * z1*zer_lup[0][2*j] * ( c_re[i][j][0][l] * c1 + c_im[i][j][0][l] * s1);

        }
      }

      for (j = 0; j < n_rad; j++)
      {

        double z1 = zer_lup[0][2*j];
        to_w += z1*z1 * c_re[0][j][0][j];

        for (k = 1; k < n_ang; k++)
        { 
          double c2 = cos( k*t );
          double s2 = sin( k*t );
          for (l = j+1; l < n_rad; l++)         // !!! attention wrong covariance coefficients
            to_w += 4*z1*zer_lup[k][k+2*l] * ( c_re[0][j][k][l] * c2 + c_im[0][j][k][l] * s2 );
        }

        for (l = 0; l < n_rad; l++)
          to_w += 2 * z1 * zer_lup[0][2*l] * c_re[0][j][0][l];

      }

      w->val[radial][theta] = to_w / ( L_max * L_max );

    }    
  }

  for (i = 0; i < n_ang; i++)
    free(zer_lup[i]);
  free(zer_lup);

  return w;

}
*/


