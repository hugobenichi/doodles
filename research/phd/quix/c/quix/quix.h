

#ifndef QUIX
  #define QUIX

  

  #include    <stdlib.h>
  #include    <stdio.h>
  #include    <math.h>
  //#include    <memory.h>

  #include    "quix/random.h"
  #include    "quix/parallel.h"
  #include    "quix/memory.h"
  #include    "quix/extern.h"
  #include    "quix/predicat.h"


  #define     PI          3.1411592654


  #define     TOS         printf("tos\n")
  #define     DEB(a,b)    printf("deb: %" # a "\n",b)


  #define     loop(index,stop)    for(index = 0; index < stop; index++)


#endif


