

#ifndef QUIX_SERIES
  #define QUIX_SERIES


  #include "quix/object/wigner.h"

  typedef 
    struct proto_series
    {
      int           siz_ang;                    
      int           siz_rad;
      double        amp_rad;
      double**      c_re;       //indices are (n,m) <-> (angular,radial)
      double**      c_im;       //indices are (n,m) <-> (angular,radial)
    } 
    series;


  EXT   series*     series_new                  (int, int, double);
  EXT   void        series_free                 (series*);   
  EXT   void        series_zernike              (double**, int, int, double);
  EXT   wigner*     series_wigner               (series*, grid*);
  EXT   double      series_negativity           (series*);


#endif


