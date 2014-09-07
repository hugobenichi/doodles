

#ifndef QUIX_IF
  #define QUIX_IF


  #define     unlikely(expr)    __builtin_expect(!!(expr), 0)
  #define     likely(expr)      __builtin_expect(!!(expr), 1)


#endif


