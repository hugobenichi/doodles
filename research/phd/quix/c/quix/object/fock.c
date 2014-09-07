#include "quix/object/fock.h"


#define   wigner_loop(index_i,index_j,s_i,s_j)        loop(index_i,s_i) loop(index_j,s_j)
#define   dmatrix_loop(index_i,index_j, m)            loop(index_i,m)   loop(index_j,m) 
#define   dmatrix_diag_loop(index_i, m)               loop(index_i,m) 

#define   Re(l,i,j)           (l->photons[i][j]->re)
#define   Im(l,i,j)           (l->photons[i][j]->im)


EXT fock_wk* fock_new
  (grid* g, int rank)
{

  int i, m = rank+1;

  fock_wk* f  = quix_object_alloc(fock_wk);
  f-> gd      = grid_copy(g);
  f-> N       = rank;
  f-> photons = quix_alloc(wigner_c**,m);
  loop(i,m)
    f-> photons[i] = quix_alloc(wigner_c*, m);

  return f;

}


EXT void fock_free
  (fock_wk* f)
{

  if ( quix_object_free(f) )
  {
    int i,j, m = f->N+1;
    if(f-> photons) {
      dmatrix_loop(i,j,m) {
        if (f-> photons[i][j]) {
          wigner_comp_free(f-> photons[i][j]);  
          f-> photons[i][j] = NULL;
        }
      }
      loop(i,m)
        free(f-> photons[i]);
      free(f-> photons);    
      f-> photons = NULL;
    } else already_null("Fock::Photons");
    grid_free(f->gd);
    f->gd = NULL;
    free(f);
  } else already_free("Fock");

}


EXT void fock_calculate
  (fock_wk* f)
{

  grid* g = f-> gd;

  generator *g_vac = generator_vacuum(),
            *g_one = generator_one();

  wigner *vac = generator_make_wigner( g_vac, g ),
         *one = generator_make_wigner( g_one, g );

  f-> photons[0][0] = wigner_comp_convert( generator_make_wigner( generator_vacuum(), g ) );
  f-> photons[1][1] = wigner_comp_convert( generator_make_wigner( generator_one(), g ) );

  generator_free(g_vac);
  generator_free(g_one);
  wigner_free(vac);
  wigner_free(one);

  int i, j, 
      a, b,
      n  = f-> N,
      m  = f-> N+1,
      Nx = g->x->n_val,
      Ny = g->y->n_val;

  double  s, t, z, v,
         *gx = g-> x-> val, 
         *gy = g-> y-> val; 

  if_cartesian(g) {

    for(i = 1; i < n; i++) {

      f-> photons[i+1][i+1] = wigner_comp_new(g);

      wigner_loop(a,b,Nx,Ny) {
        double x = gx[a],
               y = gy[b];
        z = 2 * ( x*x + y*y );
        Re(f,i+1,i+1)[a][b] = ( (z-2*i-1) * Re(f,i,i)[a][b]
                                - i * Re(f,i-1,i-1)[a][b] )/(i+1);
        Im(f,i+1,i+1)[a][b] = 0.0;
      }

    }

    loop(i,n) {

      f-> photons[0][i+1] = wigner_comp_new(g);
      f-> photons[i+1][0] = wigner_comp_new(g);
      s = sqrt(2.0/(i+1));

      wigner_loop(a,b,Nx,Ny) {
        double x = gx[a],
               y = gy[b];
        Re(f,i+1,0)[a][b] = s*( x * Re(f,i,0)[a][b] + y * Im(f,i,0)[a][b] );
        Im(f,i+1,0)[a][b] = s*( x * Im(f,i,0)[a][b] - y * Re(f,i,0)[a][b] ); 
        Re(f,0,i+1)[a][b] = s*( x * Re(f,0,i)[a][b] - y * Im(f,0,i)[a][b] ); 
        Im(f,0,i+1)[a][b] = s*( x * Im(f,0,i)[a][b] + y * Re(f,0,i)[a][b] );
      }

    }

    for (i = 1; i <  n; i++)
      for (j = i; j <  n; j++) {

        f-> photons[i][j+1] = wigner_comp_new(g); 
        f-> photons[j+1][i] = wigner_comp_new(g);
        s = sqrt(2.0*i*j/(j+1));
        t = sqrt(2.0/(j+1));

        wigner_loop(a,b,Nx,Ny) {
          double x = gx[a],
                 y = gy[b];

          z =  2 * ( x*x + y*y );
          Re(f,j+1,i)[a][b] = s/z*( -x * Re(f,j-1,i-1)[a][b] - y * Im(f,j-1,i-1)[a][b] ) 
                            - t*(i/z-1)*( x * Re(f,j,i)[a][b] + y * Im(f,j,i)[a][b] ); 
          Im(f,j+1,i)[a][b] = s/z*( -x * Im(f,j-1,i-1)[a][b] + y * Re(f,j-1,i-1)[a][b] ) 
                            - t*(i/z-1)*( x * Im(f,j,i)[a][b] - y * Re(f,j,i)[a][b] ); 
          Re(f,i,j+1)[a][b] = s/z*( -x *Re(f,i-1,j-1)[a][b] + y * Im(f,i-1,j-1)[a][b] ) 
                            - t*(i/z-1)*( x * Re(f,i,j)[a][b] - y * Im(f,i,j)[a][b] ); 
          Im(f,i,j+1)[a][b] = s/z*( -x * Im(f,i-1,j-1)[a][b] - y * Re(f,i-1,j-1)[a][b] ) 
                            - t*(i/z-1)*( x * Im(f,i,j)[a][b] + y * Re(f,i,j)[a][b] ); 
        }

        a = (Nx-1)/2;
        b = (Ny-1)/2;
        v = 1.0*(i+j+1)/(j+1);

        double x = gx[a],
               y = gy[b];

        Re(f,j+1,i)[a][b] = t*v*( x * Re(f,j,i)[a][b] + y * Im(f,j,i)[a][b] );
        Im(f,j+1,i)[a][b] = t*v*( x * Im(f,j,i)[a][b] - y * Re(f,j,i)[a][b] );
        Re(f,i,j+1)[a][b] = t*v*( x * Re(f,i,j)[a][b] - y * Im(f,i,j)[a][b] );
        Im(f,i,j+1)[a][b] = t*v*( x * Im(f,i,j)[a][b] - y * Re(f,i,j)[a][b] );
 
      }

  } else {

    for(i = 1; i < n; i++) {

      f-> photons[i+1][i+1] = wigner_comp_new(g);

      wigner_loop(a,b,Nx,Ny) {
        double r = gx[a];
        Re(f,i+1,i+1)[a][b] = ( (2*r*r-2*i-1)*(Re(f,i,i)[a][b]) - i*(Re(f,i-1,i-1)[a][b]) )/(i+1);
        Im(f,i+1,i+1)[a][b] = 0.0;
      }

    }

    loop(i,n) {

      f-> photons[0][i+1] = wigner_comp_new(g);
      f-> photons[i+1][0] = wigner_comp_new(g);
      s = sqrt(2.0/(i+1));

      wigner_loop(a,b,Nx,Ny) {
        double x = gx[a] * cos( gy[b] ),
               y = gx[a] * sin( gy[b] );
        Re(f,i+1,0)[a][b] = s*( x * Re(f,i,0)[a][b] + y * Im(f,i,0)[a][b] );
        Im(f,i+1,0)[a][b] = s*( x * Im(f,i,0)[a][b] - y * Re(f,i,0)[a][b] ); 
        Re(f,0,i+1)[a][b] = s*( x * Re(f,0,i)[a][b] - y * Im(f,0,i)[a][b] ); 
        Im(f,0,i+1)[a][b] = s*( x * Im(f,0,i)[a][b] + y * Re(f,0,i)[a][b] );
      }

    }

    for (i = 1; i <  n; i++)
      for (j = i; j <  n; j++) {

        f-> photons[i][j+1] = wigner_comp_new(g); 
        f-> photons[j+1][i] = wigner_comp_new(g);
        s = sqrt(2.0*i*j/(j+1));
        t = sqrt(2.0/(j+1));

       loop(b,Ny) { 
          double x = gx[0] * cos( gy[b] ), // !!! warning !!!
                 y = gx[0] * sin( gy[b] );
          v = 1.0*(i+j+1)/(j+1);
          Re(f,j+1,i)[0][b] = t*v*( x * Re(f,j,i)[0][b] + y * Im(f,j,i)[0][b] );
          Im(f,j+1,i)[0][b] = t*v*( x * Im(f,j,i)[0][b] - y * Re(f,j,i)[0][b] );
          Re(f,i,j+1)[0][b] = t*v*( x * Re(f,i,j)[0][b] - y * Im(f,i,j)[0][b] );
          Im(f,i,j+1)[0][b] = t*v*( x * Im(f,i,j)[0][b] - y * Re(f,i,j)[0][b] ); 
        }

        for(a = 1; a <  Nx; a++)
          loop(b,Ny) { 
            double x = gx[a] * cos( gy[b] ),
                   y = gx[a] * sin( gy[b] );
            z =  2*gx[a] * gx[a];
            Re(f,j+1,i)[a][b] = s/z*( -x * Re(f,j-1,i-1)[a][b] - y * Im(f,j-1,i-1)[a][b] ) 
                              - t*(i/z-1)*( x * Re(f,j,i)[a][b] + y * Im(f,j,i)[a][b] ); 
            Im(f,j+1,i)[a][b] = s/z*( -x * Im(f,j-1,i-1)[a][b] + y * Re(f,j-1,i-1)[a][b] ) 
                              - t*(i/z-1)*( x * Im(f,j,i)[a][b] - y * Re(f,j,i)[a][b] ); 
            Re(f,i,j+1)[a][b] = s/z*( -x * Re(f,i-1,j-1)[a][b] + y * Im(f,i-1,j-1)[a][b] ) 
                              - t*(i/z-1)*( x * Re(f,i,j)[a][b] - y * Im(f,i,j)[a][b] ); 
            Im(f,i,j+1)[a][b] = s/z*( -x * Im(f,i-1,j-1)[a][b] - y * Re(f,i-1,j-1)[a][b] ) 
                              - t*(i/z-1)*( x * Im(f,i,j)[a][b] + y * Re(f,i,j)[a][b] ); 
          }

      }

  }

}


EXT wigner* fock_wig_from_matrix
  (fock_workspace* f, dmatrix* d)
{

  wigner* w = wigner_new(f->gd);
  wigner_to_zero(w);

  int i, j, m = f->N + 1,
      x, y,
      Nx = f->gd->x->n_val,
      Ny = f->gd->x->n_val;

  wigner_loop(x,y,Nx,Ny)
    dmatrix_loop(i,j,m)
      w->val[x][y] += d->re[i][j] * f-> photons[i][j]-> re[x][y]
                    - d->im[i][j] * f-> photons[i][j]-> im[x][y];

  return w;

}


/* only work for radial grids atm */
EXT dmatrix* fock_wig_to_matrix
  (fock_workspace* f, wigner* w)
{

  dmatrix* d = dmatrix_new(f->N);

  grid* g = w->gd;
  int i, j, m = f->N + 1,
      x, y, 
      Nx = g-> x-> n_val,
      Ny = g-> y-> n_val;
  
  double norm = 2*PI * grid_cell(g);
  
  dmatrix_loop(i,j,m) {

    double  over_re = 0.0,
            over_im = 0.0,
          **phot_re = f->photons[i][j]->re,
          **phot_im = f->photons[i][j]->im;
      
    loop(x,Nx) {

      double radius = g-> x-> val[x],
             ang_re = 0.0,
             ang_im = 0.0;

      loop(y,Ny) {
        ang_re +=  w-> val[x][y] * phot_re[x][y];
        ang_im +=  w-> val[x][y] * phot_im[x][y];
      }

      over_re += radius * ang_re; 
      over_im += radius * ang_im; 

    }

    d->re[i][j] = norm * over_re;
    d->im[i][j] = norm * over_im;

  }

  return d;

}


#undef Re
#undef Im


#undef wigner_loop
#undef dmatrix_diag_loop
#undef dmatrix_loop


