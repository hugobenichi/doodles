#include <stdio.h>

char hex2char[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
int  mask = 1 + 2 + 4 + 8;

void dec2hex(int dec, char* str)
{
  *str      = hex2char[ mask & ( dec >> 4)];
  *(str+1)  = hex2char[ mask & dec ]; 
}


int main(int argc, char* argv[])
{

  int r = 0, g = 0, b = 0;
  
  char value[9] = "0x000000";

  if (argc > 3)
  {
    dec2hex( atoi(argv[1]), value+2);
    dec2hex( atoi(argv[2]), value+4);
    dec2hex( atoi(argv[3]), value+6);
  }
  
  printf("%s\n", value );
  
}
