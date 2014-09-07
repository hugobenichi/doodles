

#include "quix/tomography/series.h"


EXT void              
series_zernike       
(double** zer, int N, int S, double x)
{

  int n, s;
  double x2 = x*x;

  zer[0][0] = 1.0;                            // n = 0, s = 0
  zer[1][1] = x;                            // n = 1, s = 0

  for(n = 2; n < N; n++)                    // n = n, s = 0
  { 
    zer[n][n]   = x * zer[n-1][n-1];
    zer[n][n-2] = 0.0;
  }

  zer[0][2] = 2.0 * x2 - 1.0;                   // n = 0, s = 1;
  zer[1][3] = ( 3.0 * x2 - 2.0 )*x;   // n = 1, s = 1;

  for(s = 2; s < S; s++)                    // n = 0, s = s
  {
    double c2   = 2.0*(2.0*s - 1.0 ) / s;
    double c3   = c2 * 0.5 * (s-1.0) - (2.0*s-1.0);
    double c4   = (2*s - 3) * ( s - 1 + c3 )  - c2 * 0.5 * (s-2) * (s-2);
    zer[0][2*s] = (c2 * x2 + c3) * zer[0][2*s-2]  + c4 * zer[0][2*s-4];
  }


  for(s = 2; s < S; s++)                    // n = 1, s = s
  {
    double c2 = 2.0*(2.0*s + 1.0 ) / (s+1.0);
    double c3 = c2 * s * (s-1) /(2*s-1) - 2 * s;
    double c4 = (s-1) * (2*s - 1 + 2*c3) - c2 * 0.5 * (s-1) * (s-2);
    zer[1][1+2*s] = (c2 * x2 + c3) * zer[1][1+2*s-2]  + c4 * zer[1][1+2*s-4];
  }


  for(n = 2; n < N; n++)                    // n = n, s = s
  {
    for(s = 1; s < S; s++) 
    {
      double c2 = (n + 2.0*s)*(n + 2.0*s - 1.0) / ( s * (n + s) );
      double c3 = c2 * (s - 1)* (n + s - 1)/(n + 2*s - 2) - (n+2*s-1);
      double c4 = (n + 2*s - 3) * ( c3 + 0.5*n + s - 1 ) - c2 * 0.5 * (s-2) * (n+s-2);
      zer[n][n+2*s] = (c2 * x2 + c3) * zer[n][n+2*s-2]  + c4 * zer[n][n+2*s-4];
    }
  }

}


