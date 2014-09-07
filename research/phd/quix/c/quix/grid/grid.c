

#include "quix/grid/grid.h"


static grid* 
range2grid
(range* a, range* b, grid_type tp)
{

  grid* g = quix_object_alloc(grid);

  g-> x     = a;
  g-> y     = b;
  g-> type  = tp;

  return g;

}


EXT grid*    
grid_copy          
(grid* g)
{
  
  range *x = range_copy(g->x),
        *y = range_copy(g->y);
     
  return range2grid(x,y,g->type);
  
}


grid* 
grid_xy
(double max, int steps)
{

  int i;
  double min = -max * ( steps - 1 ) / steps;

  range* x = range_new( 2*steps - 1 );
  range* y = range_new( 2*steps - 1 );

  x-> amplitude  = max;
  y-> resolution = max/steps;
  x-> amplitude  = max;
  y-> resolution = max/steps;

  for(i = 0; i < 2*steps - 1; i++) {

    x-> val[i] = min + i * max / steps;
    y-> val[i] = x-> val[i];

  }

  return range2grid(x,y,cartesian);

}


grid* 
grid_rad
(double max, int r_steps, int a_steps)
{

  int i;

  range* r = range_new(r_steps);
  r-> amplitude  = max;
  r-> resolution = max / r_steps;

  for(i = 0; i < r_steps; i++)
    r->val[i] = i * max / r_steps;

  range* a = range_new(a_steps);
  a-> amplitude  = 2*PI;
  a-> resolution = 2*PI / a_steps;

  for(i = 0; i < a_steps; i++)
    a->val[i] = i * 2 * PI / a_steps;

  return range2grid(r,a,radial);

}


void 
grid_free
(grid* g)
{   

  if ( quix_object_free(g) )
  {
    if (g-> x && g-> y)
    {
      range_free( g-> x );
      range_free( g-> y );
      g-> x = NULL; 
      g-> y = NULL; 
    } else already_null("Grid::Range");
    free(g);
  } else already_free("Grid");

}


void 
grid_hist
(grid* g)
{

  range* swap = g-> x;

  g-> x = range_new((g-> y-> n_val) / 2);
  g-> x-> amplitude = PI;
  g-> x-> resolution = g-> y-> resolution;

  int i;
  for(i = 0; i < g-> x-> n_val; i++)
    (g-> x-> val)[i] = i * (g-> y-> resolution);

  g-> y = swap;

  range_symmetrize(g-> y);

  g-> type = histo;

}

 
double 
grid_cell
(grid* g)
{

  return fabs(g->x->val[0]-g->x->val[1]) * fabs(g->y->val[0]-g->y->val[1]);

}


