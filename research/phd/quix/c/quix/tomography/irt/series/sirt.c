

#include "quix/tomography/irt/irt_series.h"


EXT 
void 
sirt_calculate
(series* ft, marginal* m)
{

  //printf("calculating wigner harmonics with chebysnev polynomials of the second kind\n");

  int       i, j, n,
            n_ang = ft-> siz_ang,
            n_rad = ft-> siz_rad,
            N     = m -> samples;
  double    L_max = ft-> amp_rad,
           *cheby = double_alloc( (n_ang + 2*n_rad) ),
          **c_re  = ft -> c_re,
          **c_im  = ft -> c_im;

  loop(i,n_ang)
    loop(j,n_rad) {
      c_re[i][j] = 0.0;
      c_im[i][j] = 0.0;      
    }

  loop(n,N)
  {
    double x = (m -> quad[n]) / L_max;   // <--- unity disk | or take max of quad

    if ( fabs(x) > 1.0 ) 
      continue; 

    double t = m -> phase[n];

    cheby[0] = 1.0;
    cheby[1] = 2.0 * x;   //second kind chebysnev polynomials
    for (i = 2; i < n_ang + 2*n_rad; i++)
      cheby[i] = 2.0 * x * cheby[i-1] - cheby[i-2];


    loop(i,n_ang)
    {
      double c = cos( i*t ),
             s = sin( i*t );
      loop(j,n_rad) 
      {
        c_re[i][j] += c * cheby[i+2*j];
        c_im[i][j] += s * cheby[i+2*j];    
      }
    }

  }

  double norm  = 1.0/( N * PI * L_max );

  loop(i,n_ang)
    loop(j,n_rad) { 
      c_re[i][j] *= norm * (i + 2*j + 1);
      c_im[i][j] *= norm * (i + 2*j + 1);         
    }

  free(cheby);

}


EXT 
double
sirt_calculate_neg
(series* ft, marginal* m)
{

  int       i, j, n, s,
            n_rad = ft-> siz_rad,
            N     = m -> samples;

  double    L_max   = ft-> amp_rad,
            neg     = 0.0,
           *cheby   = double_alloc( 2*n_rad ),
           *coeff   = double_alloc( n_rad );


  loop(i,n_rad) 
    coeff[i] = 0.0;

  loop(n,N)
  {
    double x = (m -> quad[n]) / L_max,
           t = m -> phase[n];

    if ( fabs(x) > 1.0 ) continue; 

    cheby[0] = 1.0;
    cheby[1] = 2.0 * x;
    for (i = 2; i < 2*n_rad; i++)
      cheby[i] = 2.0 * x * cheby[i-1] - cheby[i-2];

    loop(i,n_rad) 
      coeff[i] += cheby[2*i];   
  }

  for( i = 0, s = 1; i < n_rad; i++, s *= -1)
    neg += s * coeff[i] * (2*i + 1);

  free(cheby);
  free(coeff);

  return neg /( N * PI * L_max * L_max);

}




/*
 *    calculate radon_ft coefficients and also create an estimate of standard deviation 
 *    on the value of W(0,0) for the giver max n_rad
 */
EXT   
double   
sirt_calc_neg_var      
(series* ft, marginal* m)
{

  sirt_calculate(ft,m);

  int       i, j, n, 
            s1, s2,
            N     = m -> samples,
            n_rad = ft-> siz_rad;

  double    L_max = ft-> amp_rad,
           *che_lup = double_alloc( 2*n_rad ),
           *var     = double_alloc(n_rad),
           *covar   = double_alloc(n_rad*n_rad),
           *buffer  = double_alloc(n_rad);

  loop(i, n_rad)        var[i]    = 0.0;
  loop(i, n_rad*n_rad)  covar[i]  = 0.0;
  loop(i, n_rad)        buffer[i] = ft -> c_re[0][i];

  loop(n,N) 
  {
    double x = (m -> quad[n]) / L_max; 

    if ( fabs(x) > 1.0 ) continue; 

    che_lup[0] = 1.0;
    che_lup[1] = 2.0 * x;  
    for (i = 2; i < 2*n_rad; i++)
      che_lup[i] = 2.0 * x * che_lup[i-1] - che_lup[i-2];

    loop(i, n_rad) 
    {
      double c1 = che_lup[2*i];
      var[i] += c1 * c1;
      for (j = i+1; j < n_rad; j++) 
        covar[i*n_rad+j] += c1 *che_lup[2*j]; 
    }

  }

  double norm = 1.0/( N * PI*PI * L_max*L_max ),
         neg  = 0.0;

  for (s1 = 1, i = 0; i < n_rad; i++, s1 = -s1) 
  {
    double m1 = buffer[i];
    neg += var[i] * (2*i+1)*(2*i+1) * norm - m1*m1;
    for( j = 0, s2 = s1; j < i+1; j++, s2 = -s2 );
    for (j = i+1; j < n_rad; j++, s2 = -s2)
      neg += 2 * s2 * ( covar[i*n_rad+j] * (2*i+1)*(2*j+1)*norm - m1*buffer[j] );
  }

  free(che_lup);
  free(var);
  free(covar);
  free(buffer);

  return sqrt( neg / (N-1) ) / L_max;

}


/*
 *    calculate radon_ft coefficients and also create an estimate of standard deviation 
 *    on the value of W(0,0) for the all value of n_rad up to the max n_rad
 */
EXT   
double*     
sirt_calc_neg_var_conv
(series* ft, marginal* m, double* err)
{

  sirt_calculate(ft,m);

  int       i, j, k, n, 
            s1, s2,
            N     = m -> samples,
            n_rad = ft-> siz_rad;

  double    L_max = ft-> amp_rad,
           *che_lup = double_alloc( 2*n_rad ),
           *var     = double_alloc(n_rad),
           *covar   = double_alloc(n_rad*n_rad),
           *buffer  = double_alloc(n_rad),
           *disp    = double_alloc(n_rad);

  loop(i, n_rad)        var[i]    = 0.0;
  loop(i, n_rad*n_rad)  covar[i]  = 0.0;
  loop(i, n_rad)        disp[i]   = 0.0;
  loop(i, n_rad)        buffer[i] = ft -> c_re[0][i];

  loop(n,N) 
  {
    double x = (m -> quad[n]) / L_max; 

    if ( fabs(x) > 1.0 ) continue; 

    che_lup[0] = 1.0;
    che_lup[1] = 2.0 * x;  
    for (i = 2; i < 2*n_rad; i++)
      che_lup[i] = 2.0 * x * che_lup[i-1] - che_lup[i-2];

    loop(i, n_rad)
    {
      double c1 = che_lup[2*i];
      var[i] += c1 * c1;
      for (j = i+1; j < n_rad; j++) 
        covar[i*n_rad+j] += c1 *che_lup[2*j]; 
    }

  }

  double norm = 1.0/( N * PI*PI * L_max*L_max ),
         neg  = 0.0;

  for (k = 0; k < n_rad; k++)
  {
    for (s1 = 1, i = 0; i < k; i++, s1 = -s1) 
    {
      double m1 = buffer[i];
      err[i] = fabs(m1)/L_max;
      neg += var[i] * (2*i+1)*(2*i+1) * norm - m1*m1;
      for( j = 0, s2 = s1; j < i+1; j++, s2 = -s2 );
      for (j = i+1; j < k; j++, s2 = -s2)
        neg += 2 * s2 * ( covar[i*n_rad+j] * (2*i+1)*(2*j+1)*norm - m1*buffer[j] );
    }
    disp[k] = sqrt( neg / (N-1) ) / L_max;
  }

  free(che_lup);
  free(var);
  free(covar);
  free(buffer);

  return disp;

}


