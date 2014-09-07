#include "quix/generator/generator.h"


EXT generator* generator_new
  (int p_size)
{

  generator* g = quix_object_alloc(generator);

  if (p_size > 0) g-> parameters = double_alloc(p_size);
  else            g-> parameters = NULL;

  return g;

}


EXT void generator_free
  (generator* g)
{

  if( quix_object_free(g) )
  {
    if (g-> parameters) free(g-> parameters);
    free(g);
  } else already_free("Generator");

}


EXT wigner*  generator_make_wigner
  (generator* gen, grid* g)
{

  return gen-> wig(gen, g);

}


EXT dmatrix* generator_make_dmatrix
  (generator* gen, int n)
{

  return gen-> dmat(gen, n);

}


EXT histogram* generator_make_histogram
  (generator* gen, grid* g, range* r)
{

  return gen-> hist(gen, g, r);

}


EXT marginal* generator_make_marginal
  (generator* gen, int n, double quad)
{

  return gen-> marg(gen, n, quad);

}


