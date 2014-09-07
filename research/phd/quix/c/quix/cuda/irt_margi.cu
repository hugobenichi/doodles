#include "cuda_irt.h"

#define   EPS     1.1929093e-7

/*

  constant variables declarations

*/


  __constant__    float         cutoff;
  __constant__    float         cutoff_sqrd;

  __constant__    grid_type     wg_type;
  __constant__    int           wg_x_bin;
  __constant__    int           wg_y_bin;
  __constant__    float         wg_x_rez;
  __constant__    float         wg_y_rez;
  __constant__    float         wg_x_amp;
  __constant__    float         wg_y_amp;


/*

  kernel declarations

*/

  __global__      void          compute_wigner_rd( float* quad, float* phase, float* wigner);
  __global__      void          compute_wigner_xy( float* quad, float* phase, float* wigner);
  __global__      void          compute_negativity( float* quad, float* neg); 

  __device__      float         eval_kernel(float);


EXT()  __host__ cuda_irt_margi_wk*     cuda_irt_margi_alloc
  (int n)
{
  
  cuda_irt_margi_wk* wk = (cuda_irt_margi_wk*) malloc( sizeof( cuda_irt_margi_wk ) ); 
  
  wk-> samples = n;

  size_t memory_size = n * sizeof(float2);

  cudaMallocHost( (void**) &wk-> host_data, memory_size)
  cudaMalloc( (void**) &wk-> dev_data, memory_size );

  return wk;

}


EXT()  __host__ void                   cuda_irt_margi_free
  (cuda_irt_margi_wk* wk)
{

  cudaFreeHost( wk-> host_data );

  cudaFree( wk-> dev_data );

  free( wk );

}


EXT()  __host__ void                   cuda_irt_margi_data
  (cuda_irt_margi_wk* wk, marginal* m)
{

  for( int i = 0; i < wk-> samples; i++)
    wk-> host_data[i] = make_float2( (float) m-> quad[i], (float) m-> phas[i] );

  size_t copy_size = wk-> samples * sizeof(float2);

  cudaMemcpy( wk-> dev_data, wk-> host_data, copy_size, cudaMemcpyHostToDevice  );
  
}


EXT()  __host__  void  cuda_irt_margi_grid 
  (grid* g)
{

  float wg_x_rez = (float) g-> x-> resolution;
  float wg_y_rez = (float) g-> y-> resolution;
  float wg_x_amp = (float) g-> x-> amplitude;
  float wg_y_amp = (float) g-> y-> amplitude;  

  cudaMemcpyToSymbol( "wg_type",  &g-> grid_type, sizeof(grid_type));
  cudaMemcpyToSymbol( "wg_x_bin", &g-> x-> n_val, sizeof(int));
  cudaMemcpyToSymbol( "wg_y_bin", &g-> y-> n_val, sizeof(int));
  cudaMemcpyToSymbol( "wg_x_rez", &wg_x_rez, sizeof(float));
  cudaMemcpyToSymbol( "wg_y_rez", &wg_y_rez, sizeof(float));
  cudaMemcpyToSymbol( "wg_x_amp", &wg_x_amp, sizeof(float));
  cudaMemcpyToSymbol( "wg_y_amp", &wg_y_amp, sizeof(float));

}


EXT()  __host__  void  cuda_irt_margi_cutoff
  (Real cutoff)
{

  float cut = cutoff
  float cut_sqrd = (float) cutoff*cutoff;
  
  cudaMemcpyToSymbol( "cutoff", &cut, sizeof(float));
  cudaMemcpyToSymbol( "cutoff_sqrd", &cut_sqrd , sizeof(float));

}


EXT()  __host__  wigner*                 cuda_irt_margil
  (cuda_irt_margi_wk* wk)
{

  grid* g   = 
  wigner* w = wigner_alloc


}


EXT()  __host__  Real                    cuda_irt_margi_neg
  (cuda_irt_margi_wk* wk)
{


}


// Part 2 of 2: implement the fast kernel using shared memory
__global__ void reverseArrayBlock(int *d_out, int *d_in)
{

    extern __shared__ int s_data[];
    //__shared__ int s_data[256];

    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = ( gridDim.x - 1 - blockIdx.x )* blockDim.x + threadIdx.x;

    // Load one element per thread from device memory and store it 
    // *in reversed order* into temporary shared memory
    s_data[blockDim.x - 1 - threadIdx.x] = d_in[i]; 

    // Block until all threads in the block have written their data to shared mem
    __syncthreads();

    // write the data from shared memory in forward order, 
    // but to the reversed block offset as before
    d_out[j] = s_data[threadIdx.x]; 

}


__global__  void                         computer_wigner_rd
  ( float* quad, float* phase, float* wigner)
{

  extern __shared__ float quad[]; 
  extern __shared__ float phas[];

  float r = ;   //threadIdx
  float phi = ; //blockIdx
  float x;

  float w;

  for(int i = 0; i < nantoka; i++) {

    //copy data 

    __syncthreads();

    for (int n = 0; j < loaded memory size; j++) {

      x  = r * cosf( phi - phas[n] ) - quad[n];       
      w += eval_kernel(x);

    }


  }


}

__device__  float                        eval_kernel
  (float x)
{

  float p = (fabsf(x) + EPS)* cutoff;    
  float pp = 1.0f / p * p;

#ifdef FAST_KERNEL

  return pp * ( cutoff_sqrd * ( cosf(p) + p * sinf(p) ) - 1.0f );

#else

  return pp * ( cutoff_sqrd * ( __cosf(p) + p * __sinf(p) ) - 1.0f );

#endif

}

















int main( int argc, char** argv) 
{
    // pointer for host memory and size
    int *h_a;
    int dimA = 256 * 1024; // 256K elements (1MB total)

    // pointer for device memory
    int *d_b, *d_a;

    // define grid and block size
    int numThreadsPerBlock = 256;

    // Compute number of blocks needed based on array size and desired block size
    int numBlocks = dimA / numThreadsPerBlock;  

    // Part 1 of 2: Compute the number of bytes of shared memory needed
    // This is used in the kernel invocation below
    int sharedMemSize = numThreadsPerBlock * sizeof(int);

    // allocate host and device memory
    size_t memSize = numBlocks * numThreadsPerBlock * sizeof(int);
    h_a = (int *) malloc(memSize);
    cudaMalloc( (void **) &d_a, memSize );
    cudaMalloc( (void **) &d_b, memSize );

    // Initialize input array on host
    for (int i = 0; i < dimA; ++i)
    {
        h_a[i] = i;
    }

    // Copy host array to device array
    cudaMemcpy( d_a, h_a, memSize, cudaMemcpyHostToDevice );

    // launch kernel
    dim3 dimGrid(numBlocks);
    dim3 dimBlock(numThreadsPerBlock);
    reverseArrayBlock<<< dimGrid, dimBlock, sharedMemSize >>>( d_b, d_a );
    //reverseArrayBlock<<< dimGrid, dimBlock >>>( d_b, d_a );

    // block until the device has completed
    cudaThreadSynchronize();

    // check if kernel execution generated an error
    // Check for any CUDA errors
    checkCUDAError("kernel invocation");

    // device to host copy
    cudaMemcpy( h_a, d_b, memSize, cudaMemcpyDeviceToHost );

    // Check for any CUDA errors
    checkCUDAError("memcpy");

    // verify the data returned to the host is correct
    for (int i = 0; i < dimA; i++)
    {
        assert(h_a[i] == dimA - 1 - i );
    }

    // free device memory
    cudaFree(d_a);
    cudaFree(d_b);

    // free host memory
    free(h_a);

    // If the program makes it this far, then the results are correct and
    // there are no run-time errors.  Good work!
    printf("Correct!\n");

    return 0;
}

#undef EPS
