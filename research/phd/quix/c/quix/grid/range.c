

#include "quix/grid/range.h"


range* 
range_new
(int n)
{

  //range*  r   = quix_alloc(range,1);
  range* r = quix_object_alloc(range);
  double* val = double_alloc(n);

  r-> n_val = n;
  r-> val   = val;

  return r;

}



EXT range*    
range_copy          
(range* r)
{

  int i, n = r-> n_val;

  range* r_copy = range_new(n);

  loop(i,n)
    r_copy-> val[i] = r-> val[i];

  return r_copy;

}


void 
range_free
(range *r)
{

  if( quix_object_free(r) )
  {
    if (r-> val) 
    {
      free( r-> val); 
      r-> val = NULL; 
    } else already_null("Range");    
    free(r);
  } else already_free("Range");

}


range* 
range_single
(int step, double amp)
{

  int i = 0;

  range* r = range_new(step);

  r-> amplitude  = amp;
  r-> n_val      = step;
  r-> resolution = amp / step;

  while(step - i) {
    r-> val[i] = 1.0 * i * amp / step; 
    i++;
  }

  return r;

}


range*
range_dual
(int step, double amplitude)
{

  range* r = range_new(2*step - 1);

  int i = 0;
  while(2*step - 1 - i){

    r->val[i] = - amplitude + ( i + 1 ) * amplitude / step; 
    i++;

  }

  return r;

}


void
range_symmetrize
(range* r)
{

  double* new_val = double_alloc( 2*(r-> n_val) - 1 );

  int i;

  for (i = 0; i < r-> n_val; i++ ) new_val[i + r-> n_val - 1] =  r-> val[i];
  for (i = 1; i < r-> n_val; i++ ) new_val[r-> n_val - 1 - i] = -r-> val[i];

  free(r-> val);

  r-> val   = new_val;
  r-> n_val = 2 * (r-> n_val) - 1;

}


