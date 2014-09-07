#ifndef _QUIX_CUDA_IRT_
#define _QUIX_CUDA_IRT_

#include "irt.h"
#include "cuda.h"


/*
  to build wigner function use constant memory for grid info
*/


/*
  total constant memory on quadroFX580 is 64kbyte <=> 2048 floats
  not possible to store 64*64 grid value
  cheap to recompute
*/


typedef  struct  proto_cuda_irt_histogram_workspace
{

                float2*        host_bins;
  __device__    float2*        dev_bins;
                
  
}  cuda_irt_histo_wk;


typedef  struct  proto_cuda_irt_marginal_workspace
{

                size_t        samples
                float2*       host_data;
  __device__    float2*       dev_data;
  
}  cuda_irt_margi_wk;

EXT()  __host__  void                 

EXT()  __host__  cuda_irt_histo_wk*   cuda_irt_histo_alloc  (grid*);
EXT()  __host__  void                 cuda_irt_histo_free   (cuda_irt_histo_wk*);
EXT()  __host__  void                 cuda_irt_histo_data   (cuda_irt_histo_wk*, histogram*);
EXT()  __host__  void                 cuda_irt_histo_grid   (cuda_irt_histo_wk*, grid*);
EXT()  __host__  wigner*              cuda_irt_histo_cutoff (cuda_irt_histo_wk*, Real);
EXT()  __host__  wigner*              cuda_irt_histo        (cuda_irt_histo_wk*);
EXT()  __host__  Real                 cuda_irt_histo_neg    (cuda_irt_histo_wk*);

EXT()  __host__  cuda_irt_margi_wk*   cuda_irt_margi_alloc  (int);
EXT()  __host__  void                 cuda_irt_margi_free   (cuda_irt_margi_wk*);
EXT()  __host__  void                 cuda_irt_margi_data   (cuda_irt_margi_wk*, marginal*);
EXT()  __host__  void                 cuda_irt_margi_grid   (grid*);
EXT()  __host__  void                 cuda_irt_margi_cutoff (Real);
EXT()  __host__  wigner*              cuda_irt_margi        (cuda_irt_margi_wk*);
EXT()  __host__  Real                 cuda_irt_margi_neg    (cuda_irt_margi_wk*);

#endif
