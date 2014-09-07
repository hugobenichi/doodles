

#include "quix/random.h"


static int now = -1;


void reinit_seed
  ()
{

  int t = time(NULL);

  if ( now != t ) {
    srand(t);
    now = t; 
  }

}


