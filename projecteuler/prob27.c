#include <stdio.h>
#include <stdlib.h>

void sleeve(int* table, int len)
{

  int i = 0;
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

int count(int a, int b, int* table, int len)
{ 
  int n = 0;
  int quad = b;
  //while( quad > 0 && quad < len && table[quad]  )
  while( quad > 0 && table[quad]  )
  {
  if (quad > len)
   {
    printf("overflow\n");
    break;
   }
    n++;
    quad += 2*n + a - 1;
  }
  return n;
}


int main(void)
{

  int len = 100000;
  int* table = malloc(sizeof(int) * len);

  sleeve(table, len);
  int max = 1000;
  int prod = 0;
  int best = 0;
  
  int b = 2;
  while( b < max )
  {
      //printf("%i\n",b);
    int a = -b;
    while( a < max )
    {
      //printf("  %i\n",a);
      int c = count( a, b, table, len);
      if ( c > best )
      {
        best = c;
        prod = a*b;
      }
      a++;
    }
    b++;
    while( !table[b] )
      b++;
  }
  
  printf("count: %i\nprod: %i\n",best,prod);

  free(table);

  exit(0);

}
