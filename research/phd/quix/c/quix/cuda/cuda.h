#ifndef _QUIX_CUDA_
#define _QUIX_CUDA_

void checkCUDAError
  (const char *msg)
{

  cudaError_t err = cudaGetLastError();

  if( cudaSuccess != err) {
    fprintf(stderr, "Cuda error: %s: %s.\n", msg, cudaGetErrorString( err) );
    exit(EXIT_FAILURE);
  }  
                       
}

#endif
