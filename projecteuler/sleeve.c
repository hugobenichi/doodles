#include <stdio.h>
#include <stdlib.h>

void sleeve(int* table, int len)
{

  table[0] = 0;
  table[1] = 0;
  int i = 2;
  while(len-i)
    table[i++] = 1;
        
  i = 2;
  while(len-i)
  {
    if(table[i])
    {
      int k = 2*i;
      while(k < len)
      {
        table[k] = 0;
        k += i;
      }
    }
    i++;
  }
  
}


int main(void)
{

  int len = 100000;
  int* table = malloc(sizeof(int) * len);

  sleeve(table, len);

  free(table);

  exit(0);

}
