#include <stdio.h>

void string_rev(char* str)
{

  char swap;
  char* end = str;
  while( *(end+1) )
    end++;

  while( end - str > 0)
  {
    swap = *str;
    *str = *end;
    *end = swap;
    str++;
    end--;
  }  

}

int main(int argc, char* argv[])
{

  int i = 1;
  while( argc - i )
  {
    printf("%s -> ",argv[i]);
    string_rev( argv[i] );
    printf("%s\n",argv[i]);
    i++;    
  }

}


