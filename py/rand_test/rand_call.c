#include <stdlib.h>


void seed(int s){ srand(s); }


int get(void){ return rand(); }


long int get_r(unsigned short* x){ return nrand48(x);}

