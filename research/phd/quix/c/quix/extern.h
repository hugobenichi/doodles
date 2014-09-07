

#ifndef QUIX_EXTERN
  #define QUIX_EXTERN

    #ifdef __WIN32__
      #define EXT extern __declspec(dllexport)
    #else
      #define EXT
    #endif

#endif


